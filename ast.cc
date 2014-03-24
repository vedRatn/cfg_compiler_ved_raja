
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
		CHECK_INVARIANT(loc_var_val->get_result_enum() == int_result, "Result type can only be int");
		file_buffer << loc_var_val->get_int_value() << "\n";
	}

	else
	{
		CHECK_INVARIANT(glob_var_val->get_result_enum() == int_result, 
			"Result type can only be int and float");

		if (glob_var_val == NULL)
			file_buffer << "0\n";
		else
			file_buffer << glob_var_val->get_int_value() << "\n";
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
		i = new Eval_Result_Value_Int();
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Type of a name can be int/float only", lineno);

	if (result.get_result_enum() == int_result)
	 	i->set_value(result.get_int_value());
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
	Register_Descriptor * result_register = machine_dscr_object.get_new_register();
	Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

	Icode_Stmt * load_stmt = new Move_IC_Stmt(load, opd, register_opd);

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

	Ics_Opd * register_opd = new Register_Addr_Opd(store_register);
	Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry);

	Icode_Stmt * store_stmt = new Move_IC_Stmt(store, register_opd, opd);

	store_register->clear_lra_symbol_list();
	variable_symbol_entry->update_register(store_register);

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

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Data_Type constant_data_type, int line)
{
	constant = number;
	node_data_type = constant_data_type;

	ast_num_child = zero_arity;

	lineno = line;
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
void Number_Ast<DATA_TYPE>::print(ostream & file_buffer)
{
	file_buffer << std::fixed;
	file_buffer << std::setprecision(2);

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
}

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile()
{
	Register_Descriptor * result_register = machine_dscr_object.get_new_register();
	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");
	Ics_Opd * load_register = new Register_Addr_Opd(result_register);
	Ics_Opd * opd = new Const_Opd<int>(constant);

	Icode_Stmt * load_stmt = new Move_IC_Stmt(imm_load, opd, load_register);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(load_stmt);

	Code_For_Ast & num_code = *new Code_For_Ast(ic_list, result_register);
	string str("number");
	result_register->update_symbol_information(*new Symbol_Table_Entry(str, int_data_type, -1));
	// cout<<result_register->get_name()<< " busy huwa nunber"<<endl;
	return num_code;
}

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((lra.get_register() != NULL), "Register assigned through optimize_lra cannot be null");
	Ics_Opd * load_register = new Register_Addr_Opd(lra.get_register());
	Ics_Opd * opd = new Const_Opd<int>(constant);

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
	Eval_Result & result = *new Eval_Result_Value_Int();
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

template class Number_Ast<int>;


/************************************************************************************/


Goto_Ast::Goto_Ast(int succ)
{
	successor = succ;
}

Goto_Ast::~Goto_Ast()
{}

void Goto_Ast::print(ostream & file_buffer)
{
	file_buffer << AST_SPACE << "Goto statement:\n";
	file_buffer << AST_NODE_SPACE << "Successor: "<<successor<<"\n";
}

int Goto_Ast::next_bb(){
	return successor;
}

Eval_Result & Goto_Ast::evaluate(Local_Environment & eval, ostream & file_buffer){
	print(file_buffer);
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

Code_For_Ast & Goto_Ast::compile(){
	Register_Descriptor * faltu_register = machine_dscr_object.get_new_register();
	Ics_Opd * opd = new Const_Opd<int>(successor);

	Icode_Stmt * goto_stml = new Control_Flow_IC_Stmt(goto_op, opd);
	list<Icode_Stmt *> ic_list;
	ic_list.push_back(goto_stml);

	Code_For_Ast & goto_code = *new Code_For_Ast(ic_list, faltu_register);
	return goto_code;
}

Code_For_Ast & Goto_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Register_Descriptor * faltu_register = machine_dscr_object.get_new_register();
	Ics_Opd * opd = new Const_Opd<int>(successor);

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

	if(comp == NONE)
		return true;

	CHECK_INVARIANT((rhs != NULL), "Rhs of Relational_Ast cannot be null");

	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
		"Assignment statement data type not compatible");
}


Data_Type Relational_Ast::get_data_type(){
	return lhs->get_data_type();
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
		return lhs->evaluate(eval_env, file_buffer);
	}

	Eval_Result & result=*new Eval_Result_Value_Int();
	Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
	Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);

	CHECK_INVARIANT(lhsResult.is_variable_defined(), 
			"Variable should be defined to be on lhs");
	CHECK_INVARIANT(rhsResult.is_variable_defined(), 
			"Variable should be defined to be on rhs");

	if(comp == LE){
		if(lhsResult.get_int_value() <= rhsResult.get_int_value())
			result.set_value(1);
		else
			result.set_value(0);
	}else if(comp == GE){
		if(lhsResult.get_int_value() >= rhsResult.get_int_value())
			result.set_value(1);
		else
			result.set_value(0);
	}else if(comp == EQ){
		if(lhsResult.get_int_value() == rhsResult.get_int_value())
			result.set_value(1);
		else
			result.set_value(0);
	}else if(comp == NE){
		if(lhsResult.get_int_value() != rhsResult.get_int_value())
			result.set_value(1);
		else
			result.set_value(0);
	}else if(comp == GT){
		if(lhsResult.get_int_value() > rhsResult.get_int_value())
			result.set_value(1);
		else
			result.set_value(0);
	}else if(comp == LT){
		if(lhsResult.get_int_value() < rhsResult.get_int_value())
			result.set_value(1);
		else
			result.set_value(0);
	}
	return_value = (result.get_int_value()==1?true:false);
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
	if(lhs_register->find_symbol_entry_in_list(*new Symbol_Table_Entry(my_str, int_data_type, -1))){
		// cout<<"found1 "<<lhs_register->get_name()<<endl;
		lhs_register->clear_lra_symbol_list();
	}
	if(rhs_register->find_symbol_entry_in_list(*new Symbol_Table_Entry(my_str, int_data_type, -1))){
		// cout<<"found2"<<rhs_register->get_name()<<endl;
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
		if(typeid(*lhs)==typeid(Number_Ast<int>))
			return true;
	}
	return false;
}

Symbol_Table_Entry & Relational_Ast::get_symbol_entry(){
	if(comp == NONE){
		if(typeid(*lhs)!=typeid(Number_Ast<int>)){
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
	file_buffer << AST_SPACE << "If_Else statement:";
	// cout<<"Here i call rel "<<endl;
	rel->print(file_buffer);
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "True Successor: "<<true_goto->get_successor()<<"\n";
	file_buffer << AST_NODE_SPACE << "False Successor: "<<false_goto->get_successor()<<"\n";
}

Eval_Result & If_Else_Ast::evaluate(Local_Environment & eval , ostream & file_buffer){
	print(file_buffer);
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

Code_For_Ast & If_Else_Ast::compile(){
	Register_Descriptor * faltu_register = machine_dscr_object.get_new_register();
	Code_For_Ast & rel_code = rel->compile();
	Ics_Opd * lhs_opd = new Register_Addr_Opd(rel_code.get_reg());

	Ics_Opd * rhs_opd = new Register_Addr_Opd(machine_dscr_object.spim_register_table[zero]);
	Ics_Opd * bne_res_opd = new Const_Opd<int>(true_goto->get_successor());
	Ics_Opd * goto_opd = new Const_Opd<int>(false_goto->get_successor());

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
	Ics_Opd * bne_res_opd = new Const_Opd<int>(true_goto->get_successor());
	Ics_Opd * goto_opd = new Const_Opd<int>(false_goto->get_successor());

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

/************************************************************************************/

