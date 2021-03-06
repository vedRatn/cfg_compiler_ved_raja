
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

#ifndef AST_HH
#define AST_HH

#include<string>
#include<iostream>
#include<iomanip>
#include<typeinfo>
#include<list>

#define AST_SPACE "         "
#define AST_NODE_SPACE "            "
#define AST_SUB_NODE_SPACE "               "
#define AST_NODE_NODE_SPACE "               "

using namespace std;

enum COMP_ENUM { LE, GE, EQ, NE, LT, GT, NONE };

class Ast;

class Ast
{
protected:
	typedef enum
	{
		zero_arity = 0,
		unary_arity = 1,
		binary_arity = 2
	}Ast_Arity;

	Data_Type node_data_type;
	Ast_Arity ast_num_child;

	int lineno;

public:
	Ast();
	~Ast();

	virtual Data_Type get_data_type();
	virtual bool check_ast();

	virtual int get_successor();
	virtual bool get_return_value();

	virtual Symbol_Table_Entry & get_symbol_entry();

	virtual void print(ostream & file_buffer) = 0;
	virtual void print_value(Local_Environment & eval_env, ostream & file_buffer);

	virtual Eval_Result & get_value_of_evaluation(Local_Environment & eval_env);
	virtual void set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result);
	virtual Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer) = 0;

	virtual Code_For_Ast & compile() = 0;
	virtual Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra) = 0;
	virtual Code_For_Ast & create_store_stmt(Register_Descriptor * store_register);
	virtual COMP_ENUM get_comp();
	virtual bool is_number();
	virtual Register_Descriptor * get_register();
	virtual void free_register(Register_Descriptor * destination_reg_descr);
	virtual void update_register(Register_Descriptor * result_reg_descr);
	virtual bool is_trivial();
	virtual int next_bb();
	virtual int checkSuccessor(list < int > & allIds);

};

class Assignment_Ast:public Ast
{
	Ast * lhs;
	Ast * rhs;

public:
	Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line);
	~Assignment_Ast();

	bool check_ast();

	void print(ostream & file_buffer);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);

	int next_bb();

	int checkSuccessor(list < int > & allIds);


};

class Name_Ast:public Ast
{
	Symbol_Table_Entry * variable_symbol_entry;

public:
	Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line);
	~Name_Ast();

	Data_Type get_data_type();
	Symbol_Table_Entry & get_symbol_entry();

	void print(ostream & file_buffer);

	void print_value(Local_Environment & eval_env, ostream & file_buffer);
	Eval_Result & get_value_of_evaluation(Local_Environment & eval_env);
	void set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
	Register_Descriptor * get_register();
	void free_register(Register_Descriptor * destination_reg_descr);
	void update_register(Register_Descriptor * result_reg_descr);
	bool is_number();
	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
	Code_For_Ast & create_store_stmt(Register_Descriptor * store_register);
};

class Number_Ast:public Ast
{
	Value_Type constant;

public:
	Number_Ast(Value_Type number, Data_Type constant_data_type, int line);
	~Number_Ast();

	Data_Type get_data_type();

	void print(ostream & file_buffer);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
	bool is_number();
	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

class Return_Ast:public Ast
{

public:
	Return_Ast(int line);
	~Return_Ast();

	void print(ostream & file_buffer);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);

	int next_bb();
	int checkSuccessor(list < int > & allIds);
};

class Goto_Ast: public Ast
{
private:
	int successor;	
public:
	Goto_Ast(int succ);
	~Goto_Ast();
	int next_bb();
	int checkSuccessor(list < int > & allIds);
	void print(ostream & file_buffer);
	int get_successor();
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);

};

class Relational_Ast: public Ast
{
private:
	Ast * lhs;
	Ast * rhs;
	COMP_ENUM comp;
	bool return_value;
public:
	bool get_return_value();
	Relational_Ast(Ast * temp_lhs, Ast * temp_rhs, COMP_ENUM cmp);
	Relational_Ast(Ast * temp_lhs);
	Data_Type get_data_type();
	string get_string_value(Local_Environment & eval);
	bool evaluateReturnValue(Local_Environment & eval);
	void print(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
	bool check_ast();

	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
	COMP_ENUM get_comp();
	bool is_number();
	Symbol_Table_Entry & get_symbol_entry();
};

class If_Else_Ast: public Ast
{
private:
	Ast * rel;
	Ast * true_goto;
	Ast * false_goto;
public:
	If_Else_Ast(Ast * rel_temp, Ast * true_goto_temp, Ast * false_goto_temp);
	~If_Else_Ast();
	int next_bb();
	int checkSuccessor(list < int > & allIds);
	void print(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);

};

class Type_Cast_Ast: public Ast
{
private:
	Data_Type dest_type;
	Ast * ast;
public:
	Type_Cast_Ast(Ast * ast, Data_Type dt);
	Data_Type get_data_type();
	void print(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
	//bool isNumber();

	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
	bool is_number();
	Symbol_Table_Entry & get_symbol_entry();
	bool is_trivial();
};


class Arithmetic_Expr_Ast: public Ast
{
public:	
	Arithmetic_Expr_Ast();
	~Arithmetic_Expr_Ast();

	virtual Data_Type get_data_type() = 0;
	virtual bool check_ast() = 0;
	virtual int get_successor();
	virtual bool get_return_value();
	virtual void print(ostream & file_buffer) = 0;
	virtual void print_value(Local_Environment & eval_env, ostream & file_buffer);

	virtual Eval_Result & get_value_of_evaluation(Local_Environment & eval_env);
	virtual void set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result);
	virtual Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer) = 0;
	virtual int next_bb();
	virtual int checkSuccessor(list < int > & allIds);

	Code_For_Ast & compile() = 0;
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra) = 0;
	//bool isNumber();
};


class Plus_Ast: public Arithmetic_Expr_Ast
{
private:
	Ast * lhs;
	Ast * rhs;
public:

	Plus_Ast(Ast * l, Ast * r);
	void print(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
	bool check_ast();
	Data_Type get_data_type();

	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

class Minus_Ast: public Arithmetic_Expr_Ast
{
private:
	Ast * lhs;
	Ast * rhs;
public:

	Minus_Ast(Ast * l, Ast * r);
	void print(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
	bool check_ast();
	Data_Type get_data_type();

	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

class Division_Ast: public Arithmetic_Expr_Ast
{
private:
	Ast * lhs;
	Ast * rhs;
public:

	Division_Ast(Ast * l, Ast * r);
	void print(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
	bool check_ast();
	Data_Type get_data_type();

	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

class Multiplication_Ast: public Arithmetic_Expr_Ast
{
private:
	Ast * lhs;
	Ast * rhs;
public:

	Multiplication_Ast(Ast * l, Ast * r);
	void print(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
	bool check_ast();
	Data_Type get_data_type();

	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

class Unary_Ast: public Arithmetic_Expr_Ast
{
private:
	Ast * ast;
public:

	Unary_Ast(Ast * ast);
	void print(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
	bool check_ast();
	Data_Type get_data_type();

	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

#endif
