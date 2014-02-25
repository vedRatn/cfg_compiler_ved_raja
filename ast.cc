
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

/*bool Assignment_Ast::isNumber(){
	return false;
}
*/
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
	Eval_Result * loc_var_val = eval_env.get_variable_value(variable_name);
	Eval_Result * glob_var_val = interpreter_global_table.get_variable_value(variable_name);

	file_buffer << AST_SPACE << variable_name << " : ";

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
			report_internal_error("Result type can only be int and float");
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
	Eval_Result * i;
	if (result.get_result_enum() == int_result)
	{
		i = new Eval_Result_Value(int_result);
	 	i->set_value(result.get_value());
	}else if(result.get_result_enum() == float_result){
		i = new Eval_Result_Value(float_result);
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

/*bool Name_Ast::isNumber(){
	return false;
}*/

///////////////////////////////////////////////////////////////////////////////


Number_Ast::Number_Ast(Value_Type number, Data_Type constant_data_type)
{
	node_data_type = constant_data_type;
	if(node_data_type == int_data_type){
		constant.i = number.i;
	}else if(node_data_type == float_data_type){
		constant.f = number.f;
	}
}


Number_Ast::~Number_Ast()
{}


Data_Type Number_Ast::get_data_type()
{
	return node_data_type;
}


void Number_Ast::print_ast(ostream & file_buffer)
{
	if(node_data_type == int_data_type){
		file_buffer << "Num : " << constant.i;
	}else if(node_data_type == float_data_type){
		file_buffer << "Num : " ;
		std::cout << std::fixed;
    	cout.precision(2);
    	std::cout << constant.f;
	}

}


Eval_Result & Number_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Value_Type vt;
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

/*bool Number_Ast::isNumber(){
	return true;
}*/

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
	Eval_Result & result = *new Eval_Result_Value(int_result);
	return result;
}

int Return_Ast::next_bb(){
	return -1;
}

int Return_Ast::checkSuccessor(list < int > & allIds){
	return 0;
}

/*bool Return_Ast::isNumber(){
	return false;
}*/
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

/*bool Goto_Ast::isNumber(){
	return false;
}*/
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
	if(comp == NONE){
		return lhs->get_data_type();
	}else{
		return node_data_type;
	}
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
		Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
		if(lhsResult.is_variable_defined() == false){
			report_error("Variable should be defined before its use", NOLINE);
		}

		return lhsResult;
	}

	Eval_Result & result=*new Eval_Result_Value(int_result);
	Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
	Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);

	if(lhsResult.is_variable_defined() == false){
		report_error("Variable should be defined to be on lhs", NOLINE);
	}
	if(rhsResult.is_variable_defined() == false){
		report_error("Variable should be defined to be on rhs", NOLINE);
	}

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

bool Relational_Ast::check_ast(int line){
	if(comp == NONE){
		node_data_type = lhs->get_data_type();
		return true;
	}
	else if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = int_data_type;
		return true;
	}

	report_error("Relational statement data type not compatible", line);
}

/*bool Relational_Ast::isNumber(){
	if(comp == NONE && lhs->isNumber())
		return true;
	else
		return false;
}*/

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

/*bool If_Else_Ast::isNumber(){
	return false;
}
*/
/************************************************************************************/

Type_Cast_Ast::Type_Cast_Ast(Ast * ast, Data_Type dt){
	this->ast = ast;
	dest_type = dt;
	node_data_type = dt;
}

void Type_Cast_Ast::print_ast(ostream & file_buffer){
	ast->print_ast(file_buffer);
}

Eval_Result & Type_Cast_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result & eval = ast->evaluate(eval_env,file_buffer);
	if(eval.is_variable_defined() == false){
			report_error("Variable should be defined before its use", NOLINE);
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

/*bool Type_Cast_Ast::isNumber(){
	return false;
}*/

//////////////////////////////////////////////////////////////////////////////

Arithmetic_Expr_Ast::Arithmetic_Expr_Ast(){}

Arithmetic_Expr_Ast::~Arithmetic_Expr_Ast(){}

Data_Type Arithmetic_Expr_Ast::get_data_type()
{
	report_internal_error("Should not reach, Arithmetic_Expr_Ast : get_data_type");
}

void Arithmetic_Expr_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	report_internal_error("Should not reach, Arithmetic_Expr_Ast : print_value");
}

Eval_Result & Arithmetic_Expr_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	report_internal_error("Should not reach, Arithmetic_Expr_Ast : get_value_of_evaluation");
}

void Arithmetic_Expr_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	report_internal_error("Should not reach, Arithmetic_Expr_Ast : set_value_of_evaluation");
}

int Arithmetic_Expr_Ast::next_bb(){
	report_internal_error("Should not reach, Arithmetic_Expr_Ast : next_bb");
}

int Arithmetic_Expr_Ast::get_successor(){
	report_internal_error("Should not reach, Arithmetic_Expr_Ast : get_successor");
}

bool Arithmetic_Expr_Ast::get_return_value(){
	report_internal_error("Should not reach, Arithmetic_Expr_Ast : get_return_value");
}

int Arithmetic_Expr_Ast::checkSuccessor(list < int > & allIds){
	report_internal_error("Should not reach, Arithmetic_Expr_Ast : checkSuccessor");
}

/*bool Arithmetic_Expr_Ast::isNumber(){
	return false;
}*/

//////////////////////////////////////////////////////////////////////////////

Plus_Ast::Plus_Ast(Ast * l, Ast * r){
	this->lhs = l;
	this->rhs = r;
}

bool Plus_Ast::check_ast(int line){
	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	report_error("Plus statement data type not compatible", line);
}

void Plus_Ast::print_ast(ostream & file_buffer){
	file_buffer << "\n";file_buffer << AST_NODE_SPACE << "Arith: PLUS\n";
	file_buffer << AST_NODE_NODE_SPACE << "LHS (";
	lhs->print_ast(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_NODE_NODE_SPACE << "RHS (";
	rhs->print_ast(file_buffer);
	file_buffer << ")";
}

Eval_Result & Plus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	// print_ast(file_buffer);
	Eval_Result  * eval;
	Value_Type vt;
	Eval_Result & lhsEval = lhs->evaluate(eval_env,file_buffer);
	Eval_Result & rhsEval = rhs->evaluate(eval_env,file_buffer);
	if(lhsEval.is_variable_defined() == false || rhsEval.is_variable_defined()== false){
		report_error("Variable should be defined before its use", NOLINE);
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


///////////////////////////////////////////////////////////////////////////////////////////

Minus_Ast::Minus_Ast(Ast * l , Ast * r){
	this->lhs = l;
	this->rhs = r;
}

bool Minus_Ast::check_ast(int line){
	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	report_error("Minus statement data type not compatible", line);
}

void Minus_Ast::print_ast(ostream & file_buffer){
	file_buffer << "\n";file_buffer << AST_NODE_SPACE << "Arith: MINUS\n";
	file_buffer << AST_NODE_NODE_SPACE << "LHS (";
	lhs->print_ast(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_NODE_NODE_SPACE << "RHS (";
	rhs->print_ast(file_buffer);
	file_buffer << ")";
}

Eval_Result & Minus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	// print_ast(file_buffer);
	Eval_Result * eval;
	Value_Type vt;
	Eval_Result & lhsEval = lhs->evaluate(eval_env,file_buffer);
	Eval_Result & rhsEval = rhs->evaluate(eval_env,file_buffer);
	if(lhsEval.is_variable_defined() == false || rhsEval.is_variable_defined()== false){
		report_error("Variable should be defined before its use", NOLINE);
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

///////////////////////////////////////////////////////////////////////////////////////

Division_Ast::Division_Ast(Ast * l , Ast * r){
	this->lhs = l;
	this->rhs = r;
}

bool Division_Ast::check_ast(int line){
	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}
	else
		report_error("Division statement data type not compatible", line);
}

void Division_Ast::print_ast(ostream & file_buffer){
	file_buffer << "\n";file_buffer << AST_NODE_SPACE << "Arith: DIV\n";
	file_buffer << AST_NODE_NODE_SPACE << "LHS (";
	lhs->print_ast(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_NODE_NODE_SPACE << "RHS (";
	rhs->print_ast(file_buffer);
	file_buffer << ")";
}

Eval_Result & Division_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	// print_ast(file_buffer);
	Eval_Result * eval;
	Value_Type vt;
	Eval_Result & lhsEval = lhs->evaluate(eval_env,file_buffer);
	Eval_Result & rhsEval = rhs->evaluate(eval_env,file_buffer);
	if(lhsEval.is_variable_defined() == false || rhsEval.is_variable_defined()== false){
		report_error("Variable should be defined before its use", NOLINE);
	}
	if(node_data_type == int_data_type){
		if(rhsEval.get_value().i == 0){
			report_error("Division by 0", NOLINE);
		}
		eval = new Eval_Result_Value(int_result);
		vt.i = lhsEval.get_value().i / rhsEval.get_value().i;
		eval->set_value(vt);
	}else if(node_data_type == float_data_type){
		if(rhs->evaluate(eval_env,file_buffer).get_value().f == 0){
			report_error("Division by 0", NOLINE);
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

/////////////////////////////////////////////////////////////////////////////////////////

Multiplication_Ast::Multiplication_Ast(Ast * l , Ast * r){
	this->lhs = l;
	this->rhs = r;
}

bool Multiplication_Ast::check_ast(int line){
	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	report_error("Multiplication statement data type not compatible", line);
}

void Multiplication_Ast::print_ast(ostream & file_buffer){
	file_buffer << "\n";file_buffer << AST_NODE_SPACE << "Arith: MULT\n";
	file_buffer << AST_NODE_NODE_SPACE << "LHS (";
	lhs->print_ast(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_NODE_NODE_SPACE << "RHS (";
	rhs->print_ast(file_buffer);
	file_buffer << ")";
}

Eval_Result & Multiplication_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	// print_ast(file_buffer);
	Eval_Result * eval;
	Value_Type vt;
	Eval_Result & lhsEval = lhs->evaluate(eval_env,file_buffer);
	Eval_Result & rhsEval = rhs->evaluate(eval_env,file_buffer);
	if(lhsEval.is_variable_defined() == false || rhsEval.is_variable_defined()== false){
		report_error("Variable should be defined before its use", NOLINE);
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

///////////////////////////////////////////////////////////////////////////////////////////////

Unary_Ast::Unary_Ast(Ast * a){
	this->ast = a;
}

bool Unary_Ast::check_ast(int line){
	node_data_type = ast->get_data_type();
	return true;
}

void Unary_Ast::print_ast(ostream & file_buffer){
	file_buffer << "\n";file_buffer << AST_NODE_SPACE << "Arith: UMINUS\n";
	file_buffer << AST_NODE_NODE_SPACE << "LHS (";
	ast->print_ast(file_buffer);
	file_buffer << ")";
}

Eval_Result & Unary_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	// print_ast(file_buffer);
	Eval_Result * eval;
	Value_Type vt;
	Eval_Result & astEval = ast->evaluate(eval_env,file_buffer);
	if(astEval.is_variable_defined()==false){
		report_error("Variable should be defined before its use", NOLINE);
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
