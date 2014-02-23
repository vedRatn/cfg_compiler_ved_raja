
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

#define AST_SPACE "         "
#define AST_NODE_SPACE "            "
#define AST_NODE_NODE_SPACE "               "

using namespace std;

enum COMP_ENUM { LE, GE, EQ, NE, LT, GT, NONE };

class Ast;

class Ast
{
protected:
	Data_Type node_data_type;
public:
	Ast();
	~Ast();

	virtual Data_Type get_data_type();
	virtual bool check_ast(int line);
	virtual int get_successor();
	virtual bool get_return_value();
	virtual void print_ast(ostream & file_buffer) = 0;
	virtual void print_value(Local_Environment & eval_env, ostream & file_buffer);

	virtual Eval_Result & get_value_of_evaluation(Local_Environment & eval_env);
	virtual void set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result);
	virtual Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer) = 0;
	virtual int next_bb();
	virtual int checkSuccessor(list < int > & allIds);
};

class Assignment_Ast:public Ast
{
	Ast * lhs;
	Ast * rhs;

public:
	Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs);
	~Assignment_Ast();

	Data_Type get_data_type();
	bool check_ast(int line);

	void print_ast(ostream & file_buffer);

	int next_bb();

	int checkSuccessor(list < int > & allIds);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Name_Ast:public Ast
{
	
	Symbol_Table_Entry variable_symbol_entry;
	string variable_name;
public:

	Name_Ast(string & name, Symbol_Table_Entry & var_entry);
	~Name_Ast();

	Data_Type get_data_type();

	void print_ast(ostream & file_buffer);
	string get_string_value(Local_Environment & eval);
	void print_value(Local_Environment & eval_env, ostream & file_buffer);
	Eval_Result & get_value_of_evaluation(Local_Environment & eval_env);
	void set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

template <class T>
class Number_Ast:public Ast
{
	T constant;

public:
	Number_Ast(T number, Data_Type constant_data_type);
	~Number_Ast();

	Data_Type get_data_type();
	string get_string_value(Local_Environment & eval);
	void print_ast(ostream & file_buffer);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Return_Ast:public Ast
{

public:
	Return_Ast();
	~Return_Ast();

	void print_ast(ostream & file_buffer);
	int next_bb();
	int checkSuccessor(list < int > & allIds);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
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
	void print_ast(ostream & file_buffer);
	int get_successor();
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
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
	void print_ast(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
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
	void print_ast(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};



class Type_Cast_Ast: public Ast
{
private:
	Data_Type dest_type;
	Ast * ast;
public:
	Type_Cast_Ast(Ast * ast);
	void print_ast(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};


class Arithmetic_Expr_Ast: public Ast
{
public:	
	Arithmetic_Expr_Ast();
	~Arithmetic_Expr_Ast();

	virtual Data_Type get_data_type() = 0;
	virtual bool check_ast(int line) = 0;
	virtual int get_successor() = 0;
	virtual bool get_return_value() = 0;
	virtual void print_ast(ostream & file_buffer) = 0;
	virtual void print_value(Local_Environment & eval_env, ostream & file_buffer) = 0;

	virtual Eval_Result & get_value_of_evaluation(Local_Environment & eval_env) = 0;
	virtual void set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result) = 0;
	virtual Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer) = 0;
	virtual int next_bb() = 0;
	virtual int checkSuccessor(list < int > & allIds) = 0;
};


class Plus_Ast: public Arithmetic_Expr_Ast
{
private:
	Ast * lhs;
	Ast * rhs;
public:

	Plus_Ast(Ast * l, Ast * r);
	void print_ast(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Minus_Ast: public Arithmetic_Expr_Ast
{
private:
	Ast * lhs;
	Ast * rhs;
public:

	Minus_Ast(Ast * l, Ast * r);
	void print_ast(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Division_Ast: public Arithmetic_Expr_Ast
{
private:
	Ast * lhs;
	Ast * rhs;
public:

	Division_Ast(Ast * l, Ast * r);
	void print_ast(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Multiplication_Ast: public Arithmetic_Expr_Ast
{
private:
	Ast * lhs;
	Ast * rhs;
public:

	Multiplication_Ast(Ast * l, Ast * r);
	void print_ast(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Unary_Ast: public Arithmetic_Expr_Ast
{
private:
	Ast * ast;
public:

	Unary_Ast(Ast * ast);
	void print_ast(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

#endif
