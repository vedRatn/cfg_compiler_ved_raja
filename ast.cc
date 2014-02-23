
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

using namespace std;

#include"user-options.hh"
#include"error-display.hh"
#include"local-environment.hh"

#include"symbol-table.hh"
#include"ast.hh"

Ast::Ast()
{}

Ast::~Ast()
{}

bool Ast::check_ast(int line)
{
	report_internal_error("Should not reach, Ast : check_ast");
}

Data_Type Ast::get_data_type()
{
	report_internal_error("Should not reach, Ast : get_data_type");
}

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	report_internal_error("Should not reach, Ast : print_value");
}

Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	report_internal_error("Should not reach, Ast : get_value_of_evaluation");
}

void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	report_internal_error("Should not reach, Ast : set_value_of_evaluation");
}

int Ast::next_bb(){
	report_internal_error("Should not reach, Ast : next_bb");
}

int Ast::get_successor(){
	report_internal_error("Should not reach, Ast : get_successor");
}

bool Ast::get_return_value(){
	report_internal_error("Should not reach, Ast : get_return_value");
}

int Ast::checkSuccessor(list < int > & allIds){
	report_internal_error("Should not reach, Ast : checkSuccessor");
}

////////////////////////////////////////////////////////////////

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs)
{
	lhs = temp_lhs;
	rhs = temp_rhs;
}

Assignment_Ast::~Assignment_Ast()
{
	delete lhs;
	delete rhs;
}

Data_Type Assignment_Ast::get_data_type()
{
	return node_data_type;
}

bool Assignment_Ast::check_ast(int line)
{
	// cout<<"line "<<line<<"is gonna be checked"<<endl;
	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	report_error("Assignment statement data type not compatible", line);
}

void Assignment_Ast::print_ast(ostream & file_buffer)
{
	// cout<<"did i come here instead?"<<endl;
	file_buffer << AST_SPACE << "Asgn:\n";

	file_buffer << AST_NODE_SPACE"LHS (";
	lhs->print_ast(file_buffer);
	file_buffer << ")\n";

	file_buffer << AST_NODE_SPACE << "RHS (";
	rhs->print_ast(file_buffer);
	file_buffer << ")\n";
}

Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result & result = rhs->evaluate(eval_env, file_buffer);

	if (result.is_variable_defined() == false)
		report_error("Variable should be defined to be on rhs", NOLINE);

	lhs->set_value_of_evaluation(eval_env, result);

	// Print the result

	print_ast(file_buffer);

	lhs->print_value(eval_env, file_buffer);

	return result;
}

int Assignment_Ast::next_bb(){
	// print_ast(std::cout);
	return 0;
}

int Assignment_Ast::checkSuccessor(list < int > & allIds){
	return 0;
}

/////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry)
{
	variable_name = name;
	variable_symbol_entry = var_entry;
}

Name_Ast::~Name_Ast()
{}

Data_Type Name_Ast::get_data_type()
{
	return variable_symbol_entry.get_data_type();
}

void Name_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "Name : " << variable_name;
}

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result_Value * loc_var_val = eval_env.get_variable_value(variable_name);
	Eval_Result_Value * glob_var_val = interpreter_global_table.get_variable_value(variable_name);

	file_buffer << AST_SPACE << variable_name << " : ";

	if (!eval_env.is_variable_defined(variable_name) && !interpreter_global_table.is_variable_defined(variable_name))
		file_buffer << "undefined";

	else if (eval_env.is_variable_defined(variable_name) && loc_var_val != NULL)
	{
		if (loc_var_val->get_result_enum() == int_result)
			file_buffer << loc_var_val->get_value() << "\n";
		else
			report_internal_error("Result type can only be int and float");
	}

	else
	{
		if (glob_var_val->get_result_enum() == int_result)
		{
			if (glob_var_val == NULL)
				file_buffer << "0\n";
			else
				file_buffer << glob_var_val->get_value() << "\n";
		}
		else
			report_internal_error("Result type can only be int and float");
	}
	file_buffer << "\n\n";
}

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	if (eval_env.does_variable_exist(variable_name))
	{
		Eval_Result * result = eval_env.get_variable_value(variable_name);
		return *result;
	}

	Eval_Result * result = interpreter_global_table.get_variable_value(variable_name);
	return *result;
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	Eval_Result_Value * i;
	if (result.get_result_enum() == int_result)
	{
		i = new Eval_Result_Value_Int();
	 	i->set_value(result.get_value());
	}

	if (eval_env.does_variable_exist(variable_name))
		eval_env.put_variable_value(*i, variable_name);
	else
		interpreter_global_table.put_variable_value(*i, variable_name);
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	return get_value_of_evaluation(eval_env);
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Data_Type constant_data_type)
{
	constant = number;
	node_data_type = constant_data_type;
}

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::~Number_Ast()
{}

template <class DATA_TYPE>
Data_Type Number_Ast<DATA_TYPE>::get_data_type()
{
	return node_data_type;
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::print_ast(ostream & file_buffer)
{
	file_buffer << "Num : " << constant;
}

template <class DATA_TYPE>
Eval_Result & Number_Ast<DATA_TYPE>::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if (node_data_type == int_data_type)
	{
		Eval_Result & result = *new Eval_Result_Value_Int();
		result.set_value(constant);

		return result;
	}

	if(node_data_type == float_data_type)
	{
		Eval_Result & result = *new Eval_Result_Value_Float();
		result.set_value(constant);

		return result;
	}
}

///////////////////////////////////////////////////////////////////////////////

Return_Ast::Return_Ast()
{}

Return_Ast::~Return_Ast()
{}

void Return_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << AST_SPACE << "Return <NOTHING>\n";
}

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	print_ast(file_buffer);
	Eval_Result & result = *new Eval_Result_Value_Int();
	return result;
}

int Return_Ast::next_bb(){
	return -1;
}

int Return_Ast::checkSuccessor(list < int > & allIds){
	return 0;
}

template class Number_Ast<int>;


/************************************************************************************/


Goto_Ast::Goto_Ast(int succ)
{
	successor = succ;
}

Goto_Ast::~Goto_Ast()
{}

void Goto_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << AST_SPACE << "Goto statement:\n";
	file_buffer << AST_NODE_SPACE << "Successor: "<<successor<<"\n";
}

int Goto_Ast::next_bb(){
	return successor;
}

Eval_Result & Goto_Ast::evaluate(Local_Environment & eval, ostream & file_buffer){
	print_ast(file_buffer);
	file_buffer << AST_SPACE << "GOTO (BB "<<successor<<")\n\n";
	Eval_Result & result = *new Eval_Result_Value_Int();
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

Data_Type Relational_Ast::get_data_type(){
	return lhs->get_data_type();
}

void Relational_Ast::print_ast(ostream & file_buffer){
	if(comp == NONE){
		lhs->print_ast(file_buffer);
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
	lhs->print_ast(file_buffer);
	file_buffer <<")\n";
	file_buffer << AST_NODE_NODE_SPACE<< "RHS " <<"(";
	rhs->print_ast(file_buffer);
	file_buffer << ")";
}

bool Relational_Ast::get_return_value(){
	return return_value;
}

Eval_Result & Relational_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){

	if(comp == NONE){
		return lhs->evaluate(eval_env, file_buffer);
	}

	Eval_Result & result=*new Eval_Result_Value_Int();
	Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
	Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);

	if(lhsResult.is_variable_defined() == false){
		report_error("Variable should be defined to be on lhs", NOLINE);
	}
	if(rhsResult.is_variable_defined() == false){
		report_error("Variable should be defined to be on rhs", NOLINE);
	}

	if(comp == LE){
		if(lhsResult.get_value() <= rhsResult.get_value())
			result.set_value(1);
		else
			result.set_value(0);
	}else if(comp == GE){
		if(lhsResult.get_value() >= rhsResult.get_value())
			result.set_value(1);
		else
			result.set_value(0);
	}else if(comp == EQ){
		if(lhsResult.get_value() == rhsResult.get_value())
			result.set_value(1);
		else
			result.set_value(0);
	}else if(comp == NE){
		if(lhsResult.get_value() != rhsResult.get_value())
			result.set_value(1);
		else
			result.set_value(0);
	}else if(comp == GT){
		if(lhsResult.get_value() > rhsResult.get_value())
			result.set_value(1);
		else
			result.set_value(0);
	}else if(comp == LT){
		if(lhsResult.get_value() < rhsResult.get_value())
			result.set_value(1);
		else
			result.set_value(0);
	}
	return_value = (result.get_value()==1?true:false);
	return result;
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

void If_Else_Ast::print_ast(ostream & file_buffer){
	file_buffer << AST_SPACE << "If_Else statement:";
	// cout<<"Here i call rel "<<endl;
	rel->print_ast(file_buffer);
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "True Successor: "<<true_goto->get_successor()<<"\n";
	file_buffer << AST_NODE_SPACE << "False Successor: "<<false_goto->get_successor()<<"\n";
}

Eval_Result & If_Else_Ast::evaluate(Local_Environment & eval , ostream & file_buffer){
	print_ast(file_buffer);
	rel->evaluate(eval, file_buffer);
	if(rel->get_return_value())
		file_buffer << AST_SPACE << "Condition True : Goto (BB "<<true_goto->get_successor()<<")\n";
	else
		file_buffer << AST_SPACE << "Condition False : Goto (BB "<<false_goto->get_successor()<<")\n";
	file_buffer << "\n" ;
	Eval_Result & result = *new Eval_Result_Value_Int();
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

/************************************************************************************/

Type_Cast_Ast::Type_Cast_Ast(Ast * ast){
	this->ast = ast;
}

void Type_Cast_Ast::print_ast(ostream & file_buffer){
	return;
}

Eval_Result & Type_Cast_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result & eval = ast->evaluate();
	if(dest_type==int_data_type){
		Eval_Result & result = *new Eval_Result_Value_Int();
		result.set_value((int)eval.get_value)
	}


}