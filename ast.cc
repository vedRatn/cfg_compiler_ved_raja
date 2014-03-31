
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

#include<iostream>
#include<fstream>
#include<typeinfo>

using namespace std;

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"local-environment.hh"
#include"icode.hh"
#include"reg-alloc.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

Ast::Ast()
{}

Ast::~Ast()
{}

bool Ast::check_ast()
{
	stringstream msg;
	msg << "No check_ast() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Data_Type Ast::get_data_type()
{
	stringstream msg;
	msg << "No get_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Symbol_Table_Entry & Ast::get_symbol_entry()
{
	stringstream msg;
	msg << "No get_symbol_entry() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	stringstream msg;
	msg << "No print_value() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

int Ast::get_successor(){
	stringstream msg;
	msg << "No get_successor() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

bool Ast::get_return_value(){
	stringstream msg;
	msg << "No get_return_value() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	stringstream msg;
	msg << "No get_value_of_evaluation() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	stringstream msg;
	msg << "No set_value_of_evaluation() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register)
{
	stringstream msg;
	msg << "No create_store_stmt() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

COMP_ENUM Ast::get_comp(){
	stringstream msg;
	msg << "No get_comp() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

bool Ast::is_number(){
	stringstream msg;
	msg << "No is_number() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Register_Descriptor * Ast::get_register(){
	stringstream msg;
	msg << "No is_register_associated() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::free_register(Register_Descriptor * destination_reg_descr){
	stringstream msg;
	msg << "No free_register() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::update_register(Register_Descriptor * result_reg_descr){
	stringstream msg;
	msg << "No update_register() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

int Ast::next_bb(){
	stringstream msg;
	msg << "No next_bb() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

int Ast::checkSuccessor(list < int > & allIds){
	stringstream msg;
	msg << "No checkSuccessor() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

////////////////////////////////////////////////////////////////

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line)
{
	lhs = temp_lhs;
	rhs = temp_rhs;

	ast_num_child = binary_arity;
	node_data_type = void_data_type;

	lineno = line;
}

Assignment_Ast::~Assignment_Ast()
{
	delete lhs;
	delete rhs;
}

bool Assignment_Ast::check_ast()
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Assignment_Ast cannot be null");
	CHECK_INVARIANT((lhs != NULL), "Lhs of Assignment_Ast cannot be null");

	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
		"Assignment statement data type not compatible");
}

void Assignment_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "Asgn:";

	file_buffer << "\n" << AST_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")";

	file_buffer << "\n" << AST_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Assignment_Ast cannot be null");
	Eval_Result & result = rhs->evaluate(eval_env, file_buffer);

	CHECK_INPUT_AND_ABORT(result.is_variable_defined(), "Variable should be defined to be on rhs of Assignment_Ast", lineno);

	CHECK_INVARIANT((lhs != NULL), "Lhs of Assignment_Ast cannot be null");

	lhs->set_value_of_evaluation(eval_env, result);

	// Print the result

	print(file_buffer);

	lhs->print_value(eval_env, file_buffer);

	return result;
}

Code_For_Ast & Assignment_Ast::compile()
{
	/* 
		An assignment x = y where y is a variable is 
		compiled as a combination of load and store statements:
		(load) R <- y 
		(store) x <- R
		If y is a constant, the statement is compiled as:
		(imm_Load) R <- y 
		(store) x <- R
		where imm_Load denotes the load immediate operation.
	*/

	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	Code_For_Ast & load_stmt = rhs->compile();

	Register_Descriptor * load_register = load_stmt.get_reg();

	Code_For_Ast store_stmt = lhs->create_store_stmt(load_register);

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (load_stmt.get_icode_list().empty() == false)
		ic_list = load_stmt.get_icode_list();

	if (store_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, load_register);

	return *assign_stmt;
}

Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");
	// Only when rhs is a number of variable is optimization required
	//because otherwise its r2m where no regall is required
	if(rhs->get_comp() == NONE){
		lra.optimize_lra(mc_2m, lhs, rhs);
	}
	
	Code_For_Ast load_stmt = rhs->compile_and_optimize_ast(lra);

	Register_Descriptor * result_register = load_stmt.get_reg();

	Code_For_Ast store_stmt = lhs->create_store_stmt(result_register);
	if(rhs->get_comp() != NONE){
		if(lhs->get_register()!=NULL){
			// cout<<"freeing register "<<lhs->get_register()->get_name()<<endl;
			lhs->free_register(lhs->get_register()); 
		}
		lhs->update_register(result_register);
	}

	list<Icode_Stmt *> ic_list;

	if (load_stmt.get_icode_list().empty() == false)
		ic_list = load_stmt.get_icode_list();

	if (store_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, result_register);

	return *assign_stmt;
}


int Assignment_Ast::next_bb(){
	// print(std::cout);
	return 0;
}

int Assignment_Ast::checkSuccessor(list < int > & allIds){
	return 0;
}


/////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line)
{
	variable_symbol_entry = &var_entry;

	CHECK_INVARIANT((variable_symbol_entry->get_variable_name() == name),
		"Variable's symbol entry is not matching its name");

	ast_num_child = zero_arity;
	node_data_type = variable_symbol_entry->get_data_type();
	lineno = line;

}

Name_Ast::~Name_Ast()
{}

Data_Type Name_Ast::get_data_type()
{
	return variable_symbol_entry->get_data_type();
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry()
{
	return *variable_symbol_entry;
}

void Name_Ast::print(ostream & file_buffer)
{
	file_buffer << "Name : " << variable_symbol_entry->get_variable_name();
}

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	string variable_name = variable_symbol_entry->get_variable_name();

	Eval_Result * loc_var_val = eval_env.get_variable_value(variable_name);
	Eval_Result * glob_var_val = interpreter_global_table.get_variable_value(variable_name);

	file_buffer << "\n" << AST_SPACE << variable_name << " : ";

	if (!eval_env.is_variable_defined(variable_name) && !interpreter_global_table.is_variable_defined(variable_name))
		file_buffer << "undefined";

	else if (eval_env.is_variable_defined(variable_name) && loc_var_val != NULL)
	{
		if (loc_var_val->get_result_enum() == int_result){
			file_buffer << loc_var_val->get_value().i << "\n";
		}
		else if(loc_var_val->get_result_enum() == float_result){
			// file_buffer << loc_var_val->get_value().f << "\n";
			file_buffer << std::fixed;
	    	file_buffer.precision(2);
	    	file_buffer << loc_var_val->get_value().f << "\n";
		}
		else
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Result type can only be int and float");
	}

	else
	{
		if (glob_var_val->get_result_enum() == int_result)
		{
			if (glob_var_val == NULL)
				file_buffer << "0\n";
			else
				file_buffer << glob_var_val->get_value().i << "\n";
		}
		else if(glob_var_val->get_result_enum() == float_result){
			if (glob_var_val == NULL)
				file_buffer << "0\n";
			else
				file_buffer << glob_var_val->get_value().f << "\n";
		}
		else 
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Result type can only be int and float");
	}
	file_buffer << "\n";
}

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	string variable_name = variable_symbol_entry->get_variable_name();

	if (eval_env.does_variable_exist(variable_name))
	{
		CHECK_INPUT_AND_ABORT((eval_env.is_variable_defined(variable_name) == true), 
					"Variable should be defined before its use", lineno);

		Eval_Result * result = eval_env.get_variable_value(variable_name);
		return *result;
	}

	CHECK_INPUT_AND_ABORT((interpreter_global_table.is_variable_defined(variable_name) == true), 
				"Variable should be defined before its use", lineno);

	Eval_Result * result = interpreter_global_table.get_variable_value(variable_name);
	return *result;
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	Eval_Result * i;
	string variable_name = variable_symbol_entry->get_variable_name();

	if (variable_symbol_entry->get_data_type() == int_data_type)
		i = new Eval_Result_Value(int_result);
	else if(variable_symbol_entry->get_data_type() == float_data_type)
		i = new Eval_Result_Value(float_result);
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Type of a name can be int/float only", lineno);

	if (result.get_result_enum() == int_result)
	 	i->set_value(result.get_value());
	else if(result.get_result_enum() == float_result)
		i->set_value(result.get_value());
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Type of a name can be int/float only", lineno);

	if (eval_env.does_variable_exist(variable_name))
		eval_env.put_variable_value(*i, variable_name);
	else
		interpreter_global_table.put_variable_value(*i, variable_name);
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	return get_value_of_evaluation(eval_env);
}

Code_For_Ast & Name_Ast::compile()
{
	Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry);
	Register_Descriptor * result_register;
	Icode_Stmt * load_stmt;
	if(variable_symbol_entry->get_data_type() == int_data_type){
		result_register = machine_dscr_object.get_new_register();
		Ics_Opd * register_opd = new Register_Addr_Opd(result_register);
		load_stmt = new Move_IC_Stmt(load, opd, register_opd);
	}
	else if(variable_symbol_entry->get_data_type() == float_data_type){
		result_register = machine_dscr_object.get_new_float_register();
		Ics_Opd * register_opd = new Register_Addr_Opd(result_register);
		load_stmt = new Move_IC_Stmt(load_d, opd, register_opd);
	}else{
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, " Name ast compile type of variable is not valid");
	}

	list<Icode_Stmt *> ic_list;
	ic_list.push_back(load_stmt);

	Code_For_Ast & load_code = *new Code_For_Ast(ic_list, result_register);
	variable_symbol_entry->update_register(result_register);
	// cout<<result_register->get_name()<< " busy huwa name = "<<variable_symbol_entry->get_variable_name()<<endl;
	return load_code;
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register)
{
	CHECK_INVARIANT((store_register != NULL), "Store register cannot be null");
	if(store_register->get_value_type() == float_num){
		if(variable_symbol_entry->get_data_type() != float_data_type){
			// types do not match
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Types of store register and variable do not match");
		}
	}else if(store_register->get_value_type() == int_num){
		if(variable_symbol_entry->get_data_type() != int_data_type){
			// types do not match
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Types of store register and variable do not match");
		}
	}else{
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Invalid value type of store register");
	}

	Ics_Opd * register_opd = new Register_Addr_Opd(store_register);
	Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry);
	Icode_Stmt * store_stmt;
	if(variable_symbol_entry->get_data_type() == int_data_type){
		store_stmt = new Move_IC_Stmt(store, register_opd, opd);
	}else if(variable_symbol_entry->get_data_type() == float_data_type){
		store_stmt = new Move_IC_Stmt(store_d, register_opd, opd);
	}else{
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Invalid value type of store variable");
	}

	// store_register->clear_lra_symbol_list();
	// cout<<"I just freed "<<store_register->get_name()<<endl;
	// variable_symbol_entry->update_register(store_register);
	// cout<<"Here i am busying "<<store_register->get_name()<<" as register for "<<variable_symbol_entry->get_variable_name()<<endl;
/*	if(store_register->is_free())
		cout<<"is now free"<<endl;
	else
		cout<<"still busy"<<endl;
	cout<<endl;*/
	if (command_options.is_do_lra_selected() == false){
		// cout<<store_register->get_name()<< " free huwa name"<<endl;
		// variable_symbol_entry->free_register(store_register);
		store_register->clear_lra_symbol_list();
	}

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(store_stmt);

	Code_For_Ast & name_code = *new Code_For_Ast(ic_list, store_register);
	return name_code;
}

Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;;

	bool load_needed = lra.is_load_needed();

	Register_Descriptor * result_register = lra.get_register();
	CHECK_INVARIANT((result_register != NULL), "Register cannot be null");
	Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

	Icode_Stmt * load_stmt = NULL;
	if (load_needed)
	{
		Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry);

		load_stmt = new Move_IC_Stmt(load, opd, register_opd);
			
		ic_list.push_back(load_stmt);
	}

	Code_For_Ast & load_code = *new Code_For_Ast(ic_list, result_register);

	return load_code;
}

Register_Descriptor * Name_Ast::get_register(){
	return variable_symbol_entry->get_register();
}

void Name_Ast::free_register(Register_Descriptor * destination_reg_descr){
	variable_symbol_entry->free_register(destination_reg_descr);
}
void Name_Ast::update_register(Register_Descriptor * result_reg_descr){
	variable_symbol_entry->update_register(result_reg_descr);
}

///////////////////////////////////////////////////////////////////////////////

Number_Ast::Number_Ast(Value_Type number, Data_Type constant_data_type, int line)
{
	node_data_type = constant_data_type;
	if(node_data_type == int_data_type){
		constant.i = number.i;
	}else if(node_data_type == float_data_type){
		constant.f = number.f;
	}
	ast_num_child = zero_arity;

	lineno = line;
}

/*Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Data_Type constant_data_type, int line)
{
	constant = number;
	node_data_type = constant_data_type;

	ast_num_child = zero_arity;

	lineno = line;
}*/

Number_Ast::~Number_Ast()
{}

Data_Type Number_Ast::get_data_type()
{
	return node_data_type;
}

void Number_Ast::print(ostream & file_buffer)
{
	if(node_data_type == int_data_type){
		file_buffer << "Num : " << constant.i;
	}else if(node_data_type == float_data_type){
		file_buffer << "Num : " ;
		file_buffer << std::fixed;
    	file_buffer.precision(2);
    	file_buffer << constant.f;
	}

}

Eval_Result & Number_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if (node_data_type == int_data_type)
	{
		Eval_Result & result = *new Eval_Result_Value(int_result);
		result.set_value(constant);
		return result;
	}

	if( node_data_type == float_data_type)
	{
		Eval_Result & result = *new Eval_Result_Value(float_result);
		result.set_value(constant);
		return result;
	}
}

Code_For_Ast & Number_Ast::compile()
{
	Register_Descriptor * result_register;
	string str("number");
	if(get_data_type() == int_data_type){
		result_register = machine_dscr_object.get_new_register();
		result_register->update_symbol_information(*new Symbol_Table_Entry(str, int_data_type, -1));
	}else if(get_data_type() == float_data_type){
		result_register = machine_dscr_object.get_new_float_register();
		result_register->update_symbol_information(*new Symbol_Table_Entry(str, float_data_type, -1));
	}else{
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, " invalid type of number ");
	}
	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");
	Ics_Opd * load_register = new Register_Addr_Opd(result_register);
	Ics_Opd * opd;
	if(node_data_type == int_data_type)
		opd = new Const_Opd(constant, int_data_type);
	else if(node_data_type == float_data_type)
		opd = new Const_Opd(constant, float_data_type);
	Icode_Stmt * load_stmt ;
	if(get_data_type() == int_data_type){
		load_stmt = new Move_IC_Stmt(imm_load, opd, load_register);
	}else if(get_data_type() == float_data_type){
		load_stmt = new Move_IC_Stmt(imm_load_d, opd, load_register);
	}

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(load_stmt);

	Code_For_Ast & num_code = *new Code_For_Ast(ic_list, result_register);
	return num_code;
}

Code_For_Ast & Number_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((lra.get_register() != NULL), "Register assigned through optimize_lra cannot be null");
	Ics_Opd * load_register = new Register_Addr_Opd(lra.get_register());
	Ics_Opd * opd;
	if(node_data_type == int_data_type)
		opd = new Const_Opd(constant, int_data_type);
	else if(node_data_type == float_data_type)
		opd = new Const_Opd(constant, float_data_type);

	Icode_Stmt * load_stmt = new Move_IC_Stmt(imm_load, opd, load_register);

	list<Icode_Stmt *> ic_list;
	ic_list.push_back(load_stmt);

	Code_For_Ast & num_code = *new Code_For_Ast(ic_list, lra.get_register());

	return num_code;
}

///////////////////////////////////////////////////////////////////////////////

Return_Ast::Return_Ast(int line)
{
	lineno = line;
	node_data_type = void_data_type;
	ast_num_child = zero_arity;
}

Return_Ast::~Return_Ast()
{}

void Return_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "Return <NOTHING>\n";
}

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	print(file_buffer);
	Eval_Result & result = *new Eval_Result_Value(int_result);
	return result;
}

Code_For_Ast & Return_Ast::compile()
{
	Code_For_Ast & ret_code = *new Code_For_Ast();
	return ret_code;
}

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	Code_For_Ast & ret_code = *new Code_For_Ast();
	return ret_code;
}

int Return_Ast::next_bb(){
	return -1;
}

int Return_Ast::checkSuccessor(list < int > & allIds){
	return 0;
}



/************************************************************************************/


Goto_Ast::Goto_Ast(int succ)
{
	successor = succ;
}

Goto_Ast::~Goto_Ast()
{}

void Goto_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "Goto statement:\n";
	file_buffer << AST_NODE_SPACE << "Successor: "<<successor;
}

int Goto_Ast::next_bb(){
	return successor;
}

Eval_Result & Goto_Ast::evaluate(Local_Environment & eval, ostream & file_buffer){
	print(file_buffer);
	file_buffer << "\n";
	file_buffer << AST_SPACE << "GOTO (BB "<<successor<<")\n\n";
	Eval_Result & result = *new Eval_Result_Value(int_result);
	return result;
}

int Goto_Ast::get_successor(){
	return successor;
}

int Goto_Ast::checkSuccessor(list < int > & allIds){
	int ans = successor;
	for(list < int > :: iterator it = allIds.begin() ; it != allIds.end() ; it ++){
		if(successor == (int)(*it)){
			ans = 0;
		}
	}
	return ans;
}

Code_For_Ast & Goto_Ast::compile(){
	Register_Descriptor * faltu_register = machine_dscr_object.get_new_register();
	Value_Type vt;
	vt.i = successor;
	Ics_Opd * opd = new Const_Opd(vt, int_data_type);

	Icode_Stmt * goto_stml = new Control_Flow_IC_Stmt(goto_op, opd);
	list<Icode_Stmt *> ic_list;
	ic_list.push_back(goto_stml);

	Code_For_Ast & goto_code = *new Code_For_Ast(ic_list, faltu_register);
	return goto_code;
}

Code_For_Ast & Goto_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Register_Descriptor * faltu_register = machine_dscr_object.get_new_register();
	Value_Type vt;
	vt.i = successor;
	Ics_Opd * opd = new Const_Opd(vt, int_data_type);

	Icode_Stmt * goto_stml = new Control_Flow_IC_Stmt(goto_op, opd);
	list<Icode_Stmt *> ic_list;
	ic_list.push_back(goto_stml);

	Code_For_Ast & goto_code = *new Code_For_Ast(ic_list, faltu_register);
	return goto_code;
}

/************************************************************************************/

Relational_Ast::Relational_Ast(Ast * temp_lhs){
	lhs = temp_lhs;
	comp = NONE;
	return_value = false;
}

Relational_Ast::Relational_Ast(Ast * temp_lhs, Ast * temp_rhs, COMP_ENUM cmp){
	lhs = temp_lhs;
	rhs = temp_rhs;
	comp = cmp;
	return_value = false;
}

bool Relational_Ast::check_ast()
{

	CHECK_INVARIANT((lhs != NULL), "Lhs of Relational_Ast cannot be null");

	if(comp == NONE){
		node_data_type = lhs->get_data_type();
		return true;
	}
	else if (lhs->get_data_type() == rhs->get_data_type())
	{
		CHECK_INVARIANT((rhs != NULL), "Rhs of Relational_Ast cannot be null");
		node_data_type = int_data_type;
		return true;
	}

	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
		"Relational statement data type not compatible");
}


Data_Type Relational_Ast::get_data_type(){
	if(comp == NONE){
		return lhs->get_data_type();
	}else{
		return node_data_type;
	}
}

void Relational_Ast::print(ostream & file_buffer){
	if(comp == NONE){
		lhs->print(file_buffer);
		return;
	}
	file_buffer << "\n"<<AST_NODE_SPACE << "Condition: ";
	if(comp == LE)
		file_buffer << "LE\n";
	else if(comp == GE)
		file_buffer << "GE\n";
	else if(comp == EQ)
		file_buffer << "EQ\n";
	else if(comp == NE)
		file_buffer << "NE\n";
	else if(comp == GT)
		file_buffer << "GT\n";
	else if(comp == LT)
		file_buffer << "LT\n";

	file_buffer << AST_NODE_NODE_SPACE << "LHS " <<"(";
	lhs->print(file_buffer);
	file_buffer <<")\n";
	file_buffer << AST_NODE_NODE_SPACE<< "RHS " <<"(";
	rhs->print(file_buffer);
	file_buffer << ")";
}

bool Relational_Ast::get_return_value(){
	return return_value;
}

Eval_Result & Relational_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){

	if(comp == NONE){
		Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
		if(lhsResult.is_variable_defined() == false){
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Variable should be defined before its use");
		}

		return lhsResult;
	}

	Eval_Result & result=*new Eval_Result_Value(int_result);
	Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
	Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);

	CHECK_INVARIANT(lhsResult.is_variable_defined(), 
			"Variable should be defined to be on lhs");
	CHECK_INVARIANT(rhsResult.is_variable_defined(), 
			"Variable should be defined to be on rhs");

	Value_Type result_value;
	
	if(comp == LE){
		if( (lhsResult.get_result_enum()==int_result?lhsResult.get_value().i:lhsResult.get_value().f) <= (rhsResult.get_result_enum()==int_result?rhsResult.get_value().i:rhsResult.get_value().f)){
			result_value.i = 1;
			result.set_value(result_value);
		}
		else{
			result_value.i = 0;
			result.set_value(result_value);
		}
	}else if(comp == GE){
		if((lhsResult.get_result_enum()==int_result?lhsResult.get_value().i:lhsResult.get_value().f) >= (rhsResult.get_result_enum()==int_result?rhsResult.get_value().i:rhsResult.get_value().f)){
			result_value.i = 1;
			result.set_value(result_value);
		}
		else{
			result_value.i = 0;
			result.set_value(result_value);
		}
	}else if(comp == EQ){
		if((lhsResult.get_result_enum()==int_result?lhsResult.get_value().i:lhsResult.get_value().f) == (rhsResult.get_result_enum()==int_result?rhsResult.get_value().i:rhsResult.get_value().f)){
			result_value.i = 1;
			result.set_value(result_value);
		}
		else{
			result_value.i = 0;
			result.set_value(result_value);
		}
	}else if(comp == NE){
		if((lhsResult.get_result_enum()==int_result?lhsResult.get_value().i:lhsResult.get_value().f) != (rhsResult.get_result_enum()==int_result?rhsResult.get_value().i:rhsResult.get_value().f)){
			result_value.i = 1;
			result.set_value(result_value);
		}
		else{
			result_value.i = 0;
			result.set_value(result_value);
		}
	}else if(comp == GT){
		if((lhsResult.get_result_enum()==int_result?lhsResult.get_value().i:lhsResult.get_value().f) > (rhsResult.get_result_enum()==int_result?rhsResult.get_value().i:rhsResult.get_value().f)){
			result_value.i = 1;
			result.set_value(result_value);
		}
		else{
			result_value.i = 0;
			result.set_value(result_value);
		}
	}else if(comp == LT){
		if((lhsResult.get_result_enum()==int_result?lhsResult.get_value().i:lhsResult.get_value().f) < (rhsResult.get_result_enum()==int_result?rhsResult.get_value().i:rhsResult.get_value().f)){
			result_value.i = 1;
			result.set_value(result_value);
		}
		else{
			result_value.i = 0;
			result.set_value(result_value);
		}
	}
	return_value = (result.get_value().i==1?true:false);
	return result;
}

Code_For_Ast & Relational_Ast::compile(){
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	
	if(comp == NONE){
		return lhs->compile();
	}

	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");
	Code_For_Ast & lhs_stmt = lhs->compile();
	Code_For_Ast & rhs_stmt = rhs->compile();
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();

	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_register);

	Register_Descriptor * result_register = machine_dscr_object.get_new_register();
	Ics_Opd * result_opd = new Register_Addr_Opd(result_register);
	Icode_Stmt * rel_stmt;
	switch(comp){
		case LE:
			rel_stmt =  new Compute_IC_Stmt(sle, lhs_opd, rhs_opd, result_opd);
			break;
		case GE:
			rel_stmt =  new Compute_IC_Stmt(sge, lhs_opd, rhs_opd, result_opd);
			break;
		case GT:
			rel_stmt =  new Compute_IC_Stmt(sgt, lhs_opd, rhs_opd, result_opd);
			break;
		case LT:
			rel_stmt =  new Compute_IC_Stmt(slt, lhs_opd, rhs_opd, result_opd);
			break;
		case EQ:
			rel_stmt =  new Compute_IC_Stmt(seq, lhs_opd, rhs_opd, result_opd);
			break;
		case NE:
			rel_stmt =  new Compute_IC_Stmt(sne, lhs_opd, rhs_opd, result_opd);
			break;
		default: 
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Relational Ast compile shouln't reach");
			break;
	}	
	ic_list.push_back(rel_stmt);
	if (command_options.is_do_lra_selected() == false){
		// cout<<lhs_register->get_name()<< " free huwa rel"<<endl;
		// cout<<rhs_register->get_name()<< " free huwa rel"<<endl;
		// variable_symbol_entry->free_register(store_register);
		lhs_register->clear_lra_symbol_list();
		rhs_register->clear_lra_symbol_list();
	}
	string str("rel");
	result_register->update_symbol_information(*new Symbol_Table_Entry(str, int_data_type, -1));
	// cout<<result_register->get_name()<< " busy huwa relational"<<endl;
	Code_For_Ast * final_stml = new Code_For_Ast(ic_list, result_register);
	return *final_stml;
}

Code_For_Ast & Relational_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	
	if(comp == NONE){
		return lhs->compile_and_optimize_ast(lra);
	}

	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	if(lhs->get_comp() == NONE){
		/*Only in this case is optimization needed*/
		lra.optimize_lra(mc_2r, NULL, lhs);
	}
	Code_For_Ast & lhs_stmt = lhs->compile_and_optimize_ast(lra);
	if(rhs->get_comp() == NONE){
		/*Only in this case is optimization needed*/
		lra.optimize_lra(mc_2r, NULL, rhs);
	}
	Code_For_Ast & rhs_stmt = rhs->compile_and_optimize_ast(lra);
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();

	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_register);

	Register_Descriptor * result_register = machine_dscr_object.get_new_register();
	Ics_Opd * result_opd = new Register_Addr_Opd(result_register);
	Icode_Stmt * rel_stmt;
	switch(comp){
		case LE:
			rel_stmt =  new Compute_IC_Stmt(sle, lhs_opd, rhs_opd, result_opd);
			break;
		case GE:
			rel_stmt =  new Compute_IC_Stmt(sge, lhs_opd, rhs_opd, result_opd);
			break;
		case GT:
			rel_stmt =  new Compute_IC_Stmt(sgt, lhs_opd, rhs_opd, result_opd);
			break;
		case LT:
			rel_stmt =  new Compute_IC_Stmt(slt, lhs_opd, rhs_opd, result_opd);
			break;
		case EQ:
			rel_stmt =  new Compute_IC_Stmt(seq, lhs_opd, rhs_opd, result_opd);
			break;
		case NE:
			rel_stmt =  new Compute_IC_Stmt(sne, lhs_opd, rhs_opd, result_opd);
			break;
		default: 
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Relational Ast compile shouln't reach");
			break;
	}	
	ic_list.push_back(rel_stmt);
	string my_str("temp");
	// cout<<"LHS PRINTING"<<endl;
	// lhs_register->print_variables();
	if(lhs_register->is_temperory()){
		// cout<<"foundlhs and freed "<<lhs_register->get_name()<<endl;
		lhs_register->clear_lra_symbol_list();
	}
	// cout<<"RHS PRINTING"<<endl;
	// rhs_register->print_variables();
	if(rhs_register->is_temperory()){
		// cout<<"foundrhs and freed"<<rhs_register->get_name()<<endl;
		rhs_register->clear_lra_symbol_list();
	}
	// cout<<"inserting2 temp for "<<result_register->get_name()<<endl;
	result_register->update_symbol_information(*new Symbol_Table_Entry(my_str, int_data_type, -1));
	// cout<<result_register->get_name()<< " busy huwa relational"<<endl;
	Code_For_Ast * final_stml = new Code_For_Ast(ic_list, result_register);
	return *final_stml;
}

COMP_ENUM Relational_Ast::get_comp(){
	return comp;
}

bool Relational_Ast::is_number(){
	if(comp == NONE){
		if(typeid(*lhs)==typeid(Number_Ast))
			return true;
	}
	return false;
}

Symbol_Table_Entry & Relational_Ast::get_symbol_entry(){
	if(comp == NONE){
		if(typeid(*lhs)!=typeid(Number_Ast)){
			return lhs->get_symbol_entry();
		}
	}
	stringstream msg;
	msg << "No get_symbol_entry() function for " << typeid(*lhs).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

/************************************************************************************/


If_Else_Ast::If_Else_Ast(Ast * rel_temp, Ast * true_goto_temp, Ast * false_goto_temp){
	rel = rel_temp;
	true_goto = true_goto_temp;
	false_goto = false_goto_temp;
}

int If_Else_Ast::next_bb(){
	if(rel->get_return_value())
		return true_goto->get_successor();
	else
		return false_goto->get_successor();
}

void If_Else_Ast::print(ostream & file_buffer){
	file_buffer << "\n";
	file_buffer << AST_SPACE << "If_Else statement:";
	// cout<<"Here i call rel "<<endl;
	rel->print(file_buffer);
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "True Successor: "<<true_goto->get_successor()<<"\n";
	file_buffer << AST_NODE_SPACE << "False Successor: "<<false_goto->get_successor();
}

Eval_Result & If_Else_Ast::evaluate(Local_Environment & eval , ostream & file_buffer){
	print(file_buffer);
	rel->evaluate(eval, file_buffer);
	file_buffer << "\n";
	if(rel->get_return_value())
		file_buffer << AST_SPACE << "Condition True : Goto (BB "<<true_goto->get_successor()<<")\n";
	else
		file_buffer << AST_SPACE << "Condition False : Goto (BB "<<false_goto->get_successor()<<")\n";
	file_buffer << "\n" ;
	Eval_Result & result = *new Eval_Result_Value(int_result);
	return result;
}

int If_Else_Ast::checkSuccessor(list < int > & allIds){
	int ans = true_goto->get_successor();
	for(list < int > :: iterator it = allIds.begin() ; it != allIds.end() ; it ++){
		if(true_goto->get_successor() == (int)(*it)){
			ans = 0;
		}
	}
	if(ans != 0){
		return ans;
	}

	ans = false_goto->get_successor();

	for(list < int > :: iterator it = allIds.begin() ; it != allIds.end() ; it ++){
		if(false_goto->get_successor() == (int)(*it)){
			ans = 0;
		}
	}
	return ans;
}

Code_For_Ast & If_Else_Ast::compile(){
	Register_Descriptor * faltu_register = machine_dscr_object.get_new_register();
	Code_For_Ast & rel_code = rel->compile();
	Ics_Opd * lhs_opd = new Register_Addr_Opd(rel_code.get_reg());

	Ics_Opd * rhs_opd = new Register_Addr_Opd(machine_dscr_object.spim_register_table[zero]);
	Value_Type vt;
	vt.i = true_goto->get_successor();
	Ics_Opd * bne_res_opd = new Const_Opd(vt, int_data_type);
	vt.i = false_goto->get_successor();
	Ics_Opd * goto_opd = new Const_Opd(vt, int_data_type);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (rel_code.get_icode_list().empty() == false)
		ic_list = rel_code.get_icode_list();
	Icode_Stmt * bne_stml = new Control_Flow_IC_Stmt(bne, lhs_opd, rhs_opd, bne_res_opd);
	Icode_Stmt * goto_stml = new Control_Flow_IC_Stmt(goto_op,goto_opd);
	ic_list.push_back(bne_stml);
	ic_list.push_back(goto_stml);
	if (command_options.is_do_lra_selected() == false){
		// cout<<rel_code.get_reg()->get_name()<< " free huwa if"<<endl;
		// variable_symbol_entry->free_register(store_register);
		rel_code.get_reg()->clear_lra_symbol_list();
	}
	Code_For_Ast * if_else_code = new Code_For_Ast(ic_list, faltu_register);
	return *if_else_code;
}

Code_For_Ast & If_Else_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Register_Descriptor * faltu_register = machine_dscr_object.get_new_register();
	Code_For_Ast & rel_code = rel->compile_and_optimize_ast(lra);
	Ics_Opd * lhs_opd = new Register_Addr_Opd(rel_code.get_reg());

	Ics_Opd * rhs_opd = new Register_Addr_Opd(machine_dscr_object.spim_register_table[zero]);
	Value_Type vt;
	vt.i = true_goto->get_successor();
	Ics_Opd * bne_res_opd = new Const_Opd(vt, int_data_type);
	vt.i = false_goto->get_successor();
	Ics_Opd * goto_opd = new Const_Opd(vt, int_data_type);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (rel_code.get_icode_list().empty() == false)
		ic_list = rel_code.get_icode_list();
	Icode_Stmt * bne_stml = new Control_Flow_IC_Stmt(bne, lhs_opd, rhs_opd, bne_res_opd);
	Icode_Stmt * goto_stml = new Control_Flow_IC_Stmt(goto_op,goto_opd);
	ic_list.push_back(bne_stml);
	ic_list.push_back(goto_stml);
	if (command_options.is_do_lra_selected() == false){
		// cout<<rel_code.get_reg()->get_name()<< " free huwa if"<<endl;
		// variable_symbol_entry->free_register(store_register);
		rel_code.get_reg()->clear_lra_symbol_list();
	}
	if(rel_code.get_reg()->is_temperory()){
		// cout<<"found rel and freed "<<rel_code.get_reg()->get_name()<<endl;
		rel_code.get_reg()->clear_lra_symbol_list();
	}
	Code_For_Ast * if_else_code = new Code_For_Ast(ic_list, faltu_register);
	return *if_else_code;
}

/************************************************************************************/


Type_Cast_Ast::Type_Cast_Ast(Ast * ast, Data_Type dt){
	this->ast = ast;
	dest_type = dt;
	node_data_type = dt;
}

void Type_Cast_Ast::print(ostream & file_buffer){
	ast->print(file_buffer);
}

Eval_Result & Type_Cast_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result & eval = ast->evaluate(eval_env,file_buffer);
	if(eval.is_variable_defined() == false){
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Variable should be defined before its use");
	}
	Value_Type vt;
	if(dest_type==int_data_type){
		Eval_Result & result = *new Eval_Result_Value(int_result);
		if(eval.get_result_enum() == int_result){
			vt.i = (int)eval.get_value().i;
			result.set_value(vt);
			result.set_result_enum(int_result);
		}else if(eval.get_result_enum() == float_result){
			vt.i = (int)eval.get_value().f;
			result.set_value(vt);
			result.set_result_enum(int_result);
		}
		return result;
	}

	if(dest_type==float_data_type){
		Eval_Result & result = *new Eval_Result_Value(float_result);
		if(eval.get_result_enum() == int_result){
			vt.f = (float)eval.get_value().i;
			result.set_value(vt);
			result.set_result_enum(float_result);
		}else if(eval.get_result_enum() == float_result){
			vt.f = (float)eval.get_value().f;
			result.set_value(vt);
			result.set_result_enum(float_result);
		}
		return result;
	}

}

Data_Type Type_Cast_Ast::get_data_type(){
	return node_data_type;
}

Code_For_Ast & Type_Cast_Ast::compile(){
	Data_Type source_data_type = ast->get_data_type();
	if(source_data_type == dest_type){
		// No additional statements added to be added
		return ast->compile();
	}else{
		// Additional statement mtc1 or mfc1 will be added depending on the source and
		// destination type
		Code_For_Ast * final_stml;
		Code_For_Ast & ast_code = ast->compile();
		list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
		if(ast_code.get_icode_list().empty() == false){
			ic_list = ast_code.get_icode_list();
		}
		Ics_Opd * ast_opd = new Register_Addr_Opd(ast_code.get_reg());
		if(source_data_type == int_data_type && dest_type == float_data_type){
			// add a mtc1 statement
			Register_Descriptor * float_reg = machine_dscr_object.get_new_float_register();
			Ics_Opd * float_opd = new Register_Addr_Opd(float_reg);
			Icode_Stmt * mtc1_stml = new Move_IC_Stmt(mtc1, ast_opd, float_opd);
			ic_list.push_back(mtc1_stml);
			string str("type_cast");
			float_reg->update_symbol_information(*new Symbol_Table_Entry(str, int_data_type, -1));
			final_stml =  new Code_For_Ast(ic_list, float_reg);
		}else if(source_data_type == float_data_type && dest_type == int_data_type){
			// add a mfc1 statement
			Register_Descriptor * int_reg = machine_dscr_object.get_new_register();
			Ics_Opd * int_opd = new Register_Addr_Opd(int_reg);
			Icode_Stmt * mfc1_stml = new Move_IC_Stmt(mfc1, ast_opd, int_opd);
			ic_list.push_back(mfc1_stml);
			string str("type_cast");
			int_reg->update_symbol_information(*new Symbol_Table_Entry(str, int_data_type, -1));
			final_stml =  new Code_For_Ast(ic_list, int_reg);
		}else{
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Type cast compile has invalid source or destination types")
		}
		if (command_options.is_do_lra_selected() == false){
			// cout<<lhs_register->get_name()<< " free huwa rel"<<endl;
			// cout<<rhs_register->get_name()<< " free huwa rel"<<endl;
			// variable_symbol_entry->free_register(store_register);
			ast_code.get_reg()->clear_lra_symbol_list();
		}
		return *final_stml;
	}
}

Code_For_Ast & Type_Cast_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

/*bool Type_Cast_Ast::isNumber(){
	return false;
}*/

//////////////////////////////////////////////////////////////////////////////

Arithmetic_Expr_Ast::Arithmetic_Expr_Ast(){}

Arithmetic_Expr_Ast::~Arithmetic_Expr_Ast(){}

Data_Type Arithmetic_Expr_Ast::get_data_type()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Should not reach, Arithmetic_Expr_Ast : get_data_type");
}

void Arithmetic_Expr_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Should not reach, Arithmetic_Expr_Ast : print_value");
}

Eval_Result & Arithmetic_Expr_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Should not reach, Arithmetic_Expr_Ast : get_value_of_evaluation");
}

void Arithmetic_Expr_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Should not reach, Arithmetic_Expr_Ast : set_value_of_evaluation");
}

int Arithmetic_Expr_Ast::next_bb(){
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Should not reach, Arithmetic_Expr_Ast : next_bb");
}

int Arithmetic_Expr_Ast::get_successor(){
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Should not reach, Arithmetic_Expr_Ast : get_successor");
}

bool Arithmetic_Expr_Ast::get_return_value(){
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Should not reach, Arithmetic_Expr_Ast : get_return_value");
}

int Arithmetic_Expr_Ast::checkSuccessor(list < int > & allIds){
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Should not reach, Arithmetic_Expr_Ast : checkSuccessor");
}

/*bool Arithmetic_Expr_Ast::isNumber(){
	return false;
}*/

//////////////////////////////////////////////////////////////////////////////

Plus_Ast::Plus_Ast(Ast * l, Ast * r){
	this->lhs = l;
	this->rhs = r;
}

bool Plus_Ast::check_ast(){
	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Plus statement data type not compatible");
}

void Plus_Ast::print(ostream & file_buffer){
	file_buffer << "\n";file_buffer << AST_NODE_SPACE << "Arith: PLUS\n";
	file_buffer << AST_NODE_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_NODE_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

Eval_Result & Plus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	// print(file_buffer);
	Eval_Result  * eval;
	Value_Type vt;
	Eval_Result & lhsEval = lhs->evaluate(eval_env,file_buffer);
	Eval_Result & rhsEval = rhs->evaluate(eval_env,file_buffer);
	if(lhsEval.is_variable_defined() == false || rhsEval.is_variable_defined()== false){
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Variable should be defined before its use");
	}
	if(node_data_type == int_data_type){
		eval = new Eval_Result_Value(int_result);
		vt.i = lhsEval.get_value().i + rhsEval.get_value().i;
		eval->set_value(vt);
	}else if(node_data_type == float_data_type){
		eval = new Eval_Result_Value(float_result);
		vt.f = lhsEval.get_value().f + (rhsEval.get_result_enum()==int_result?rhsEval.get_value().i:rhsEval.get_value().f);
		eval->set_value(vt);
	}
	return *eval;
}

Data_Type Plus_Ast::get_data_type(){
	return node_data_type;
}

Code_For_Ast & Plus_Ast::compile(){
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");
	Code_For_Ast & lhs_stmt = lhs->compile();
	Code_For_Ast & rhs_stmt = rhs->compile();
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	// checking the data types of the two registers
	if(rhs_register->get_value_type() != lhs_register->get_value_type()){
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Plus Ast compile, the value types of the two regsiters are not the same");
	}
	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_register);
	Register_Descriptor * result_register ;
	string str("plus");
	if(lhs_register->get_value_type() == int_num){
		// addition of integers
		result_register = machine_dscr_object.get_new_register();
		result_register->update_symbol_information(*new Symbol_Table_Entry(str, int_data_type, -1));
	}else if(lhs_register->get_value_type() == float_num){
		result_register = machine_dscr_object.get_new_float_register();
		result_register->update_symbol_information(*new Symbol_Table_Entry(str, float_data_type, -1));
	}else{
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, " Invalid datatype of Plus Ast operands");
	}
	Ics_Opd * result_opd = new Register_Addr_Opd(result_register);
	Icode_Stmt * plus_stml;
	if(lhs_register->get_value_type() == int_num){
		plus_stml =  new Compute_IC_Stmt(add, lhs_opd, rhs_opd, result_opd);	
	}else{
		plus_stml = new Compute_IC_Stmt(add_d, lhs_opd, rhs_opd, result_opd);	
	}
	ic_list.push_back(plus_stml);
	if (command_options.is_do_lra_selected() == false){
		// cout<<lhs_register->get_name()<< " free huwa rel"<<endl;
		// cout<<rhs_register->get_name()<< " free huwa rel"<<endl;
		// variable_symbol_entry->free_register(store_register);
		lhs_register->clear_lra_symbol_list();
		rhs_register->clear_lra_symbol_list();
	}
	Code_For_Ast * final_stml = new Code_For_Ast(ic_list, result_register);
	return *final_stml;
}

Code_For_Ast & Plus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}

///////////////////////////////////////////////////////////////////////////////////////////

Minus_Ast::Minus_Ast(Ast * l , Ast * r){
	this->lhs = l;
	this->rhs = r;
}

bool Minus_Ast::check_ast(){
	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Minus statement data type not compatible");
}

void Minus_Ast::print(ostream & file_buffer){
	file_buffer << "\n";file_buffer << AST_NODE_SPACE << "Arith: MINUS\n";
	file_buffer << AST_NODE_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_NODE_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

Eval_Result & Minus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	// print(file_buffer);
	Eval_Result * eval;
	Value_Type vt;
	Eval_Result & lhsEval = lhs->evaluate(eval_env,file_buffer);
	Eval_Result & rhsEval = rhs->evaluate(eval_env,file_buffer);
	if(lhsEval.is_variable_defined() == false || rhsEval.is_variable_defined()== false){
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Variable should be defined before its use");
	}
	if(node_data_type == int_data_type){
		eval = new Eval_Result_Value(int_result);
		vt.i = lhsEval.get_value().i - rhsEval.get_value().i;
		eval->set_value(vt);
	}else if(node_data_type == float_data_type){
		eval = new Eval_Result_Value(float_result);
		vt.f = lhsEval.get_value().f - (rhsEval.get_result_enum()==int_result?rhsEval.get_value().i:rhsEval.get_value().f);
		eval->set_value(vt);
	}
	return *eval;
}

Data_Type Minus_Ast::get_data_type(){
	return node_data_type;
}


Code_For_Ast & Minus_Ast::compile(){
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");
	Code_For_Ast & lhs_stmt = lhs->compile();
	Code_For_Ast & rhs_stmt = rhs->compile();
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	// checking the data types of the two registers
	if(rhs_register->get_value_type() != lhs_register->get_value_type()){
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Minus Ast compile, the value types of the two regsiters are not the same");
	}
	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_register);
	Register_Descriptor * result_register ;
	string str("sub");
	if(lhs_register->get_value_type() == int_num){
		// addition of integers
		result_register = machine_dscr_object.get_new_register();
		result_register->update_symbol_information(*new Symbol_Table_Entry(str, int_data_type, -1));
	}else if(lhs_register->get_value_type() == float_num){
		result_register = machine_dscr_object.get_new_float_register();
		result_register->update_symbol_information(*new Symbol_Table_Entry(str, float_data_type, -1));
	}else{
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, " Invalid datatype of Minus Ast operands");
	}
	Ics_Opd * result_opd = new Register_Addr_Opd(result_register);
	Icode_Stmt * plus_stml;
	if(lhs_register->get_value_type() == int_num){
		plus_stml =  new Compute_IC_Stmt(sub, lhs_opd, rhs_opd, result_opd);	
	}else{
		plus_stml = new Compute_IC_Stmt(sub_d, lhs_opd, rhs_opd, result_opd);	
	}
	ic_list.push_back(plus_stml);
	if (command_options.is_do_lra_selected() == false){
		// cout<<lhs_register->get_name()<< " free huwa rel"<<endl;
		// cout<<rhs_register->get_name()<< " free huwa rel"<<endl;
		// variable_symbol_entry->free_register(store_register);
		lhs_register->clear_lra_symbol_list();
		rhs_register->clear_lra_symbol_list();
	}
	Code_For_Ast * final_stml = new Code_For_Ast(ic_list, result_register);
	return *final_stml;
}

Code_For_Ast & Minus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}

///////////////////////////////////////////////////////////////////////////////////////

Division_Ast::Division_Ast(Ast * l , Ast * r){
	this->lhs = l;
	this->rhs = r;
}

bool Division_Ast::check_ast(){
	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}
	else
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Division statement data type not compatible");
}

void Division_Ast::print(ostream & file_buffer){
	file_buffer << "\n";file_buffer << AST_NODE_SPACE << "Arith: DIV\n";
	file_buffer << AST_NODE_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_NODE_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

Eval_Result & Division_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	// print(file_buffer);
	Eval_Result * eval;
	Value_Type vt;
	Eval_Result & lhsEval = lhs->evaluate(eval_env,file_buffer);
	Eval_Result & rhsEval = rhs->evaluate(eval_env,file_buffer);
	if(lhsEval.is_variable_defined() == false || rhsEval.is_variable_defined()== false){
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Variable should be defined before its use");
	}
	if(node_data_type == int_data_type){
		if(rhsEval.get_value().i == 0){
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Division by 0");
		}
		eval = new Eval_Result_Value(int_result);
		vt.i = lhsEval.get_value().i / rhsEval.get_value().i;
		eval->set_value(vt);
	}else if(node_data_type == float_data_type){
		if(rhs->evaluate(eval_env,file_buffer).get_value().f == 0){
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Division by 0");
		}
		eval = new Eval_Result_Value(float_result);
		vt.f = (lhsEval.get_result_enum()==int_result?lhsEval.get_value().i:lhsEval.get_value().f) / 
				(rhsEval.get_result_enum()==int_result?rhsEval.get_value().i:rhsEval.get_value().f);
		eval->set_value(vt);
	}
	return *eval;
}

Data_Type Division_Ast::get_data_type(){
	return node_data_type;
}



Code_For_Ast & Division_Ast::compile(){
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");
	Code_For_Ast & lhs_stmt = lhs->compile();
	Code_For_Ast & rhs_stmt = rhs->compile();
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	// checking the data types of the two registers
	if(rhs_register->get_value_type() != lhs_register->get_value_type()){
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Division Ast compile, the value types of the two regsiters are not the same");
	}
	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_register);
	Register_Descriptor * result_register ;
	string str("div");
	if(lhs_register->get_value_type() == int_num){
		// addition of integers
		result_register = machine_dscr_object.get_new_register();
		result_register->update_symbol_information(*new Symbol_Table_Entry(str, int_data_type, -1));
	}else if(lhs_register->get_value_type() == float_num){
		result_register = machine_dscr_object.get_new_float_register();
		result_register->update_symbol_information(*new Symbol_Table_Entry(str, float_data_type, -1));
	}else{
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, " Invalid datatype of Division Ast operands");
	}
	Ics_Opd * result_opd = new Register_Addr_Opd(result_register);
	Icode_Stmt * plus_stml;
	if(lhs_register->get_value_type() == int_num){
		plus_stml =  new Compute_IC_Stmt(divi, lhs_opd, rhs_opd, result_opd);	
	}else{
		plus_stml = new Compute_IC_Stmt(div_d, lhs_opd, rhs_opd, result_opd);	
	}
	ic_list.push_back(plus_stml);
	if (command_options.is_do_lra_selected() == false){
		// cout<<lhs_register->get_name()<< " free huwa rel"<<endl;
		// cout<<rhs_register->get_name()<< " free huwa rel"<<endl;
		// variable_symbol_entry->free_register(store_register);
		lhs_register->clear_lra_symbol_list();
		rhs_register->clear_lra_symbol_list();
	}
	Code_For_Ast * final_stml = new Code_For_Ast(ic_list, result_register);
	return *final_stml;
}

Code_For_Ast & Division_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}

/////////////////////////////////////////////////////////////////////////////////////////

Multiplication_Ast::Multiplication_Ast(Ast * l , Ast * r){
	this->lhs = l;
	this->rhs = r;
}

bool Multiplication_Ast::check_ast(){
	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Multiplication statement data type not compatible");
}

void Multiplication_Ast::print(ostream & file_buffer){
	file_buffer << "\n";file_buffer << AST_NODE_SPACE << "Arith: MULT\n";
	file_buffer << AST_NODE_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_NODE_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

Eval_Result & Multiplication_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	// print(file_buffer);
	Eval_Result * eval;
	Value_Type vt;
	Eval_Result & lhsEval = lhs->evaluate(eval_env,file_buffer);
	Eval_Result & rhsEval = rhs->evaluate(eval_env,file_buffer);
	if(lhsEval.is_variable_defined() == false || rhsEval.is_variable_defined()== false){
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Variable should be defined before its use");
	}
	if(node_data_type == int_data_type){
		eval = new Eval_Result_Value(int_result);
		vt.i = lhsEval.get_value().i * rhsEval.get_value().i;
		eval->set_value(vt);
	}else if(node_data_type == float_data_type){
		eval = new Eval_Result_Value(float_result);
		vt.f = lhsEval.get_value().f * (rhsEval.get_result_enum()==int_result?rhsEval.get_value().i:rhsEval.get_value().f);
		eval->set_value(vt);
	}
	return *eval;
}

Data_Type Multiplication_Ast::get_data_type(){
	return node_data_type;
}



Code_For_Ast & Multiplication_Ast::compile(){
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");
	Code_For_Ast & lhs_stmt = lhs->compile();
	Code_For_Ast & rhs_stmt = rhs->compile();
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	// checking the data types of the two registers
	if(rhs_register->get_value_type() != lhs_register->get_value_type()){
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Multiplication Ast compile, the value types of the two regsiters are not the same");
	}
	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_register);
	Register_Descriptor * result_register ;
	string str("mul");
	if(lhs_register->get_value_type() == int_num){
		// addition of integers
		result_register = machine_dscr_object.get_new_register();
		result_register->update_symbol_information(*new Symbol_Table_Entry(str, int_data_type, -1));
	}else if(lhs_register->get_value_type() == float_num){
		result_register = machine_dscr_object.get_new_float_register();
		result_register->update_symbol_information(*new Symbol_Table_Entry(str, float_data_type, -1));
	}else{
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, " Invalid datatype of Multiplication Ast operands");
	}
	Ics_Opd * result_opd = new Register_Addr_Opd(result_register);
	Icode_Stmt * plus_stml;
	if(lhs_register->get_value_type() == int_num){
		plus_stml =  new Compute_IC_Stmt(mul, lhs_opd, rhs_opd, result_opd);	
	}else{
		plus_stml = new Compute_IC_Stmt(mul_d, lhs_opd, rhs_opd, result_opd);	
	}
	ic_list.push_back(plus_stml);
	if (command_options.is_do_lra_selected() == false){
		// cout<<lhs_register->get_name()<< " free huwa rel"<<endl;
		// cout<<rhs_register->get_name()<< " free huwa rel"<<endl;
		// variable_symbol_entry->free_register(store_register);
		lhs_register->clear_lra_symbol_list();
		rhs_register->clear_lra_symbol_list();
	}
	Code_For_Ast * final_stml = new Code_For_Ast(ic_list, result_register);
	return *final_stml;
}

Code_For_Ast & Multiplication_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}

///////////////////////////////////////////////////////////////////////////////////////////////

Unary_Ast::Unary_Ast(Ast * a){
	this->ast = a;
}

bool Unary_Ast::check_ast(){
	node_data_type = ast->get_data_type();
	return true;
}

void Unary_Ast::print(ostream & file_buffer){
	file_buffer << "\n";file_buffer << AST_NODE_SPACE << "Arith: UMINUS\n";
	file_buffer << AST_NODE_NODE_SPACE << "LHS (";
	ast->print(file_buffer);
	file_buffer << ")";
}

Eval_Result & Unary_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	// print(file_buffer);
	Eval_Result * eval;
	Value_Type vt;
	Eval_Result & astEval = ast->evaluate(eval_env,file_buffer);
	if(astEval.is_variable_defined()==false){
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Variable should be defined before its use");
	}
	if(node_data_type == int_data_type){
		eval = new Eval_Result_Value(int_result);
		vt.i = -1 * astEval.get_value().i;
		eval->set_value(vt);
	}else if(node_data_type == float_data_type){
		eval = new Eval_Result_Value(float_result);
		vt.f = -1 * astEval.get_value().f;
		eval->set_value(vt);
	}
	return *eval;
}

Data_Type Unary_Ast::get_data_type(){
	return node_data_type;
}


Code_For_Ast & Unary_Ast::compile(){
	CHECK_INVARIANT((ast != NULL), "Lhs cannot be null");
	Code_For_Ast & lhs_stmt = ast->compile();
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);
	Register_Descriptor * result_register ;
	string str("uminus");
	if(lhs_register->get_value_type() == int_num){
		// addition of integers
		result_register = machine_dscr_object.get_new_register();
		result_register->update_symbol_information(*new Symbol_Table_Entry(str, int_data_type, -1));
	}else if(lhs_register->get_value_type() == float_num){
		result_register = machine_dscr_object.get_new_float_register();
		result_register->update_symbol_information(*new Symbol_Table_Entry(str, float_data_type, -1));
	}else{
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, " Invalid datatype of Unary_Ast operands");
	}
	Ics_Opd * result_opd = new Register_Addr_Opd(result_register);
	Icode_Stmt * plus_stml;
	if(lhs_register->get_value_type() == int_num){
		plus_stml =  new Compute_IC_Stmt(neg, lhs_opd, NULL, result_opd);	
	}else{
	plus_stml = new Compute_IC_Stmt(neg_d, lhs_opd, NULL, result_opd);	
	}
	ic_list.push_back(plus_stml);
	if (command_options.is_do_lra_selected() == false){
		// cout<<lhs_register->get_name()<< " free huwa rel"<<endl;
		// cout<<rhs_register->get_name()<< " free huwa rel"<<endl;
		// variable_symbol_entry->free_register(store_register);
		lhs_register->clear_lra_symbol_list();
	}
	Code_For_Ast * final_stml = new Code_For_Ast(ic_list, result_register);
	return *final_stml;
}

Code_For_Ast & Unary_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}