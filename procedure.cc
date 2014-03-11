
/*********************************************************************************************

                                cfglp : A CFG Language Processor
                                --------------------------------

           About:

           Implemented   by  Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker    (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs306: Language  Processors  (theory and  lab)  at  IIT
           Bombay.

           Release  date  Jan  15, 2013.  Copyrights  reserved  by  Uday
           Khedker. This  implemenation  has been made  available purely
           for academic purposes without any warranty of any kind.

           Documentation (functionality, manual, and design) and related
           tools are  available at http://www.cse.iitb.ac.in/~uday/cfglp


***********************************************************************************************/

#include<string>
#include<fstream>
#include<iostream>
#include<iomanip>
           
using namespace std;

#include"error-display.hh"
#include"local-environment.hh"

#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

Procedure::Procedure(Data_Type proc_return_type, string proc_name, Symbol_Table & s)
{
	return_type = proc_return_type;
	name = proc_name;
	list<Symbol_Table_Entry *>::iterator i;
	
	for (i = s.variable_table.begin(); i != s.variable_table.end(); i++)
		local_symbol_table.push_symbol(*i);

	local_symbol_table.set_table_scope(local);
	argument_symbol_list = s;
}

Procedure::~Procedure()
{
	list<Basic_Block *>::iterator i;
	for (i = basic_block_list.begin(); i != basic_block_list.end(); i++)
		delete (*i);
}

string Procedure::get_proc_name()
{
	return name;
}

void Procedure::set_basic_block_list(list<Basic_Block *> bb_list)
{
	basic_block_list = bb_list;
}

void Procedure::set_local_list(Symbol_Table & new_list)
{
	local_symbol_table = new_list;
	local_symbol_table.set_table_scope(local);
}

void Procedure::append_local_list(Symbol_Table & new_list)
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = new_list.variable_table.begin(); i != new_list.variable_table.end(); i++){
		if(local_symbol_table.variable_in_symbol_list_check((*i)->get_variable_name())){
			report_error("Formal parameter and local variable name cannot be same", NOLINE);
		}else{
			local_symbol_table.push_symbol(*i);
		}
	}
}

Data_Type Procedure::get_return_type()
{
	return return_type;
}

bool Procedure::variable_in_symbol_list_check(string variable)
{
	return local_symbol_table.variable_in_symbol_list_check(variable);
}

Symbol_Table_Entry & Procedure::get_symbol_table_entry(string variable_name)
{
	return local_symbol_table.get_symbol_table_entry(variable_name);
}

void Procedure::print_ast(ostream & file_buffer)
{
	file_buffer <<PROC_SPACE << "Procedure: "<< name << "\n\n";
	list<Basic_Block *>::iterator i;
	for(i = basic_block_list.begin(); i != basic_block_list.end(); i++){

		if((*i)->get_statement_list().size() == 0){
			char error[200];
			sprintf(error , "Atleast one of true, false, direct successors should be set");
			string str(error);
	        report_error(str, NOLINE);
		}

		if((*i)->invalidSuccessor(basic_block_list) != 0){
			char error[200];
			sprintf(error , "bb %d doesn't exist" , (*i)->invalidSuccessor(basic_block_list));
			string str(error);
	        report_error(str, NOLINE);
		}
		
		if((*i)->get_bb_number() == basic_block_list.back()->get_bb_number()){
			if((*i)->get_statement_list().back()->next_bb() == 0){
				char error[200];
				sprintf(error , "Atleast one of true, false, direct successors should be set");
				string str(error);
		        report_error(str, NOLINE);
			}
		}
		(*i)->print_bb(file_buffer);
	}
}
	
Basic_Block & Procedure::get_start_basic_block()
{
	list<Basic_Block *>::iterator i;
	i = basic_block_list.begin();
	return **i;
}

Basic_Block * Procedure::get_next_bb(Basic_Block & current_bb)
{

	/*****************************my changes*************************************/
	return current_bb.get_next_bb(basic_block_list);
	/****************************************************************************/

	/*bool flag = false;

	list<Basic_Block *>::iterator i;
	for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
	{
		if((*i)->get_bb_number() == current_bb.get_bb_number())
		{
			flag = true;
			continue;
		}
		if (flag)
			return (*i);
	}
	
	return NULL;*/
}

Eval_Result & Procedure::evaluate(ostream & file_buffer,list<Eval_Result_Value *> argument_list)
{
	Local_Environment & eval_env = *new Local_Environment();
	local_symbol_table.create(eval_env);
	
	list<Symbol_Table_Entry *>::iterator i;
	list<Eval_Result_Value *>::iterator j;
	for (i = argument_symbol_list.variable_table.begin(),j = argument_list.begin(); i != argument_symbol_list.variable_table.end(); i++,j++){
		eval_env.put_variable_value(*(*j),(*i)->get_variable_name());
	}

	Eval_Result * result = NULL;

	file_buffer << PROC_SPACE << "Evaluating Procedure << " << name << " >>\n";
	file_buffer << LOC_VAR_SPACE << "Local Variables (before evaluating):\n";
	eval_env.print(file_buffer);
	file_buffer << "\n";
	
	Basic_Block * current_bb = &(get_start_basic_block());
	while (current_bb)
	{
		result = &(current_bb->evaluate(eval_env, file_buffer));
		current_bb = get_next_bb(*current_bb);		
	}

	file_buffer << "\n\n";
	file_buffer << LOC_VAR_SPACE << "Local Variables (after evaluating) Function: << "<< name << " >>\n";

	if(result->is_variable_defined()){
		eval_env.put_variable_value(*result, "return");
	}

	eval_env.print(file_buffer);

	return *result;
}

bool Procedure::check_parameters_definition_vs_declaration(Symbol_Table & new_list){
	list<Symbol_Table_Entry *>::iterator i,j;
	if(argument_symbol_list.variable_table.empty()){
		if(!new_list.variable_table.empty()){
			return false;
		}else{
			return true;
		}
	}else{
		if(new_list.variable_table.empty())
			return false;
	}
	if(new_list.variable_table.size() != argument_symbol_list.variable_table.size()){
		return false;
	}
	for (i = new_list.variable_table.begin(), j = argument_symbol_list.variable_table.begin() ; i != new_list.variable_table.end() && j != argument_symbol_list.variable_table.end(); i++ ,j++){
		if((*i)->get_variable_name() != (*j)->get_variable_name() || (*i)->get_data_type() != (*j)->get_data_type()){
			return false;
		}
	}
	return true;
}