
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

%scanner ../scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc


%union 
{
	int integer_value;
	std::string * string_value;
	list<Ast *> * ast_list;
	Ast * ast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	Basic_Block * basic_block;
	list<Basic_Block *> * basic_block_list;
	Procedure * procedure;
};

%token <integer_value> INTEGER_NUMBER
%token <string_value> BASIC_BLOCK
%token <string_value> FLOAT_NUMBER
%token <string_value> NAME
%token RETURN INTEGER FLOAT DOUBLE VOID
%token IF ELSE GOTO ASSIGN_OP
%left ne eq
%left lt le gt ge
%left '+' '-'
%left '/' '*'


/*%type <symbol_table> variable_declaration_statement_list
%type <symbol_entry> variable_declaration_statement
%type <symbol_table> function_declaration_statement_list
%type <symbol_entry> function_declaration_statement
%type <symbol_table> function_parameter_list
%type <symbol_entry> function_parameter
%type <ast_list> function_call_list
%type <ast> function_call
%type <ast_list> function_argument_list
%type <ast> function_argument
%type <basic_block_list> basic_block_list
%type <basic_block> basic_block
%type <ast_list> executable_statement_list
%type <ast_list> assignment_statement_list
%type <ast>	if_statement
%type <ast> goto_statement
%type <ast> assignment_statement
%type <ast> relational_statement
%type <ast> arithmetic_expression
%type <ast> variable
%type <ast> constant*/

%start program

%%

program:
	variable_declaration_statement_list function_declaration_statement_list
   {
   	//	cout<<"main aaya var_decl mein1"<<endl;
   /*
		
   */}
   procedure_list
   {/*

   */}
|
	variable_declaration_statement_list
	{
	//	cout<<"main aaya var_decl mein2"<<endl;
	}
	procedure_list
	{

	}
|
	function_declaration_statement_list
	{
	//	cout<<"main aaya fun_decl mein3"<<endl;
	}
	procedure_list
	{

	}
|
	procedure_list
	{

	}

// 	declaration_statement_list procedure_name
// 	{
// 		program_object.set_global_table(*$1);
// 		return_statement_used_flag = false;				// No return statement in the current procedure till now
// 	}
// 	procedure_body
// 	{

// 		program_object.set_procedure_map(*current_procedure);

// 		if ($1)
// 			$1->global_list_in_proc_map_check(get_line_number());

// 		delete $1;
// 	}
// |
// 	procedure_name
// 	{
// 		return_statement_used_flag = false;				// No return statement in the current procedure till now
// 	}
// 	procedure_body
// 	{
// 		program_object.set_procedure_map(*current_procedure);
// 	}

;

procedure_list:
	procedure_name procedure_body
   {/*

   */}
|
	procedure_list procedure_name procedure_body
   {/*

   */}
;

procedure_name:
	NAME '(' function_parameter_list ')'
   {/*
		current_procedure = new Procedure(void_data_type, *$1);
   */}
|
	NAME '(' ')'
   {/*
		current_procedure = new Procedure(void_data_type, *$1);
   */}

;

procedure_body:
	'{' variable_declaration_statement_list
   {/*
		current_procedure->set_local_list(*$2);
		delete $2;
   */}
	basic_block_list '}'
   {/*
		
		// if (return_statement_used_flag == false)
		// {
		// 	int line = get_line_number();
		// 	report_error("Atleast 1 basic block should have a return statement", line);
		// }
		
		current_procedure->set_basic_block_list(*$4);
		if(program_object.max_bb_call > program_object.max_bb){
			char error[200];
			sprintf(error , "bb %d doesn't exist" , program_object.max_bb_call);
			string str(error);
	        report_error(str, NOLINE);
		}
		delete $4;
   */}
|
	'{' basic_block_list '}'
   {/*
		
		// if (return_statement_used_flag == false)
		// {
		// 	int line = get_line_number();
		// 	report_error("Atleast 1 basic block should have a return statement", line);
		// }
		
		//cout<<program_object.program_object.max_bb<<":"<<program_object.max_bb_call<<endl;
		current_procedure->set_basic_block_list(*$2);
		if(program_object.max_bb_call > program_object.max_bb){
			char error[200];
			sprintf(error , "bb %d doesn't exist" , program_object.max_bb_call);
			string str(error);
	        report_error(str, NOLINE);
		}
		delete $2;
   */}
;

function_declaration_statement_list:
	function_declaration_statement
   {/*

   */}
|
	function_declaration_statement_list function_declaration_statement
   {/*

   */}
;

variable_declaration_statement_list:
	variable_declaration_statement
   {/*
		int line = get_line_number();
		program_object.variable_in_proc_map_check($1->get_variable_name(), line);

		string var_name = $1->get_variable_name();
		if (current_procedure && current_procedure->get_proc_name() == var_name)
		{
			int line = get_line_number();
			report_error("Variable name cannot be same as procedure name", line);
		}

		$$ = new Symbol_Table();
		$$->push_symbol($1);
   */}
|
	variable_declaration_statement_list variable_declaration_statement
   {/*
		// if declaration is local then no need to check in global list
		// if declaration is global then this list is global list

		int line = get_line_number();
		program_object.variable_in_proc_map_check($2->get_variable_name(), line);

		string var_name = $2->get_variable_name();
		if (current_procedure && current_procedure->get_proc_name() == var_name)
		{
			int line = get_line_number();
			report_error("Variable name cannot be same as procedure name", line);
		}

		if ($1 != NULL)
		{
			if($1->variable_in_symbol_list_check(var_name))
			{
				int line = get_line_number();
				report_error("Variable is declared twice", line);
		  	}

			$$ = $1;
		}

		else
			$$ = new Symbol_Table();

		$$->push_symbol($2);
   */}
;

variable_declaration_statement:
	INTEGER NAME ';'

   {
 //  		cout<<"main aaya var_decl ke bache int mein1"<<endl;
   /*
		$$ = new Symbol_Table_Entry(*$2, int_data_type);

		delete $2;
   */}
|
	FLOAT NAME ';'

   {
   //		cout<<"main aaya var_decl ke bache float mein1"<<endl;

   	/*
		$$ = new Symbol_Table_Entry(*$2, float_data_type);

		delete $2;
   */}
|
	DOUBLE NAME ';'
   {/*
		$$ = new Symbol_Table_Entry(*$2, float_data_type);

		delete $2;	
   */}
;

function_declaration_statement:
	VOID NAME '(' function_parameter_list ')' ';'
   {/*

   */}
|
	VOID NAME '(' ')' ';'
   {/*

   */}
|
	INTEGER NAME '(' function_parameter_list ')' ';'
   {/*

   */}
|
	INTEGER NAME '(' ')' ';'
   {/*

   */}
|
	FLOAT NAME '(' function_parameter_list ')' ';'
   {
   	//	cout<<"main aaya fun_decl ke bache float mein1"<<endl;
   /*

   */}
|
	FLOAT NAME '(' ')' ';'
   {
   	//	cout<<"main aaya fun_decl ke bache float mein1"<<endl;
   /*

   */}
|
	DOUBLE NAME '(' function_parameter_list ')' ';'
   {/*

   */}
|
	DOUBLE NAME '(' ')' ';'
   {/*

   */}

;

function_parameter_list:
	function_parameter
   {/*

   */}
|
	function_parameter ',' function_parameter_list
   {/*

   */}
;

function_parameter:
	INTEGER NAME 

   {/*
	
   */}
|
	FLOAT NAME 

   {/*
		
   */}
|
	DOUBLE NAME 
   {/*
		
   */}
;

basic_block_list:
	basic_block_list basic_block
   {/*
		if (!$2)
		{
			int line = get_line_number();
			report_error("Basic block doesn't exist", line);
		}

		bb_strictly_increasing_order_check($$, $2->get_bb_number());

		$$ = $1;
		$$->push_back($2);
   */}
|
	basic_block
   {/*
		if (!$1)
		{
			int line = get_line_number();
			report_error("Basic block doesn't exist", line);
		}

		$$ = new list<Basic_Block *>;
		$$->push_back($1);
   */}
;

basic_block:
	BASIC_BLOCK ':' executable_statement_list
   {/*
//		if (*$2 != "bb")
//		{
//			int line = get_line_number();
//			report_error("Not basic block lable", line);
//		}
		
	
		string bbl(*$1);
		if(atoi(bbl.substr(4, bbl.length()-1).c_str()) > program_object.max_bb)
			program_object.max_bb = atoi(bbl.substr(4, bbl.length()-1).c_str());
		if (atoi(bbl.substr(4, bbl.length()-1).c_str()) < 2)
		{
			int line = get_line_number();
			report_error("Illegal basic block lable", line);
		}
	
		if ($3 != NULL)
			$$ = new Basic_Block(atoi(bbl.substr(4, bbl.length()-1).c_str()), *$3);
		else
		{
			list<Ast *> * ast_list = new list<Ast *>;
			$$ = new Basic_Block(atoi(bbl.substr(4, bbl.length()-1).c_str()), *ast_list);
		}

		delete $3;
   */}
;

return_statement:
	RETURN ';'
	{

	}
|
	RETURN relational_statement ';'
	{

	}
;

executable_statement_list:
	assignment_statement
	{

	}
|
	function_call ';'
	{

	}
|
	assignment_statement executable_statement_list
	{

	}
|
	function_call ';' executable_statement_list
	{

	} 
|
	return_statement
	{

	}
|
	if_statement
	{

	}
|
	goto_statement
	{

	}
;

function_call:
	NAME '(' function_argument_list ')'
	{

	}
|
	NAME '(' ')'
	{

	}
;

function_argument_list:
	function_argument
	{

	}
|
	function_argument_list ',' function_argument
	{

	}
;

function_argument:
	relational_statement
	{

	}
;

if_statement:
	IF '(' relational_statement ')' goto_statement ELSE goto_statement
   {/*
		$$ = new If_Else_Ast($3, $5, $7);
   */}
;

goto_statement:
	GOTO BASIC_BLOCK ';'

   {/*
		string str(*$2);
		if(atoi(str.substr(4, str.length()-1).c_str()) > program_object.max_bb_call)
			program_object.max_bb_call = atoi(str.substr(4, str.length()-1).c_str());
		$$ = new Goto_Ast(atoi(str.substr(4, str.length()-1).c_str()));
   */}
;

relational_statement:



// 	variable
// 	{
// 		//cout<<"line1 = "<<get_line_number()<<endl;
// 		$$ = new Relational_Ast($1);
// 	}
// |
// 	constant
// 	{
// 		//cout<<"line2 = "<<get_line_number()<<endl;
// 		$$ = new Relational_Ast($1);
// 	}

	arithmetic_expression
   {/*
		$$ = new Relational_Ast($1);
   */}
|
	relational_statement le relational_statement
   {/*
		//cout<<"line3 = "<<get_line_number()<<endl;
		$$ = new Relational_Ast($1, $3, LE);
		int line = get_line_number();
		$$->check_ast(line);
   */}
|
	relational_statement ge relational_statement
   {/*
		//cout<<"line4 = "<<get_line_number()<<endl;
		$$ = new Relational_Ast($1, $3, GE);
		int line = get_line_number();
		$$->check_ast(line);
   */}
|
	relational_statement gt relational_statement
   {/*
		//cout<<"line5 = "<<get_line_number()<<endl;
		$$ = new Relational_Ast($1, $3, GT);
		int line = get_line_number();
		$$->check_ast(line);
   */}
|
	relational_statement lt relational_statement
   {/*
		//cout<<"line6 = "<<get_line_number()<<endl;
		$$ = new Relational_Ast($1, $3, LT);
		int line = get_line_number();
		$$->check_ast(line);
   */}
|
	relational_statement eq relational_statement
   {/*
		//cout<<"line7 = "<<get_line_number()<<endl;
		$$ = new Relational_Ast($1, $3, EQ);
		int line = get_line_number();
		$$->check_ast(line);
   */}
|
	relational_statement ne relational_statement
   {/*
		//cout<<"line8 = "<<get_line_number()<<endl;
		$$ = new Relational_Ast($1, $3, NE);
		int line = get_line_number();
		$$->check_ast(line);
   */}
;

arithmetic_expression:
	constant
   {/*
		$$ = $1;
   */}
|
	variable
   {/*
		$$ = $1;
   */}
|
	function_call
	{

	}
|
	'(' relational_statement ')'
   {/*
		$$ = $2;
   */}
|
	'(' FLOAT ')' function_call
   {/*
		$$ = new Type_Cast_Ast($4, float_data_type);
   */}
|
	'(' INTEGER ')' function_call
   {/*
		$$ = new Type_Cast_Ast($4, int_data_type);
   */}
|
	'(' DOUBLE ')' function_call
   {/*
		$$ = new Type_Cast_Ast($4, float_data_type);
   */}

|
	'(' FLOAT ')' variable
   {/*
		$$ = new Type_Cast_Ast($4, float_data_type);
   */}
|
	'(' INTEGER ')' variable
   {/*
		$$ = new Type_Cast_Ast($4, int_data_type);
   */}
|
	'(' DOUBLE ')' variable
   {/*
		$$ = new Type_Cast_Ast($4, float_data_type);
   */}
|
	'(' INTEGER ')' '(' arithmetic_expression ')'
   {/*
		$$ = new Type_Cast_Ast($5, int_data_type);
   */}
|
	'(' FLOAT ')' '(' relational_statement ')'
   {/*
		$$ = new Type_Cast_Ast($5, float_data_type);
   */}
|
	'(' DOUBLE ')' '(' relational_statement ')'	
   {/*
		$$ = new Type_Cast_Ast($5, float_data_type);
   */}
|
	'-' arithmetic_expression
	{

	}
|
	arithmetic_expression '+' arithmetic_expression
   {/*
		$$ = new Plus_Ast($1, $3);
		int line = get_line_number();
		$$->check_ast(line);
   */}
|
	arithmetic_expression '-' arithmetic_expression
   {/*
		$$ = new Minus_Ast($1, $3);
		int line = get_line_number();
		$$->check_ast(line);
   */}
|
	arithmetic_expression '*' arithmetic_expression
   {/*
		$$ = new Multiplication_Ast($1, $3);
		int line = get_line_number();
		$$->check_ast(line);
   */}
|
	arithmetic_expression '/' arithmetic_expression
   {/*
		$$ = new Division_Ast($1, $3);
		int line = get_line_number();
		$$->check_ast(line);
   */}
;

assignment_statement:
	variable ASSIGN_OP relational_statement ';'
   {/*
		$$ = new Assignment_Ast($1, $3);
		int line = get_line_number();
		//cout<<"ass line = "<<line<<endl;
		$$->check_ast(line);
   */}
;

variable:
	NAME
   {
//   		cout<<"NAME DETECTED = "<<(*$1)<<endl;
   /*
		Symbol_Table_Entry var_table_entry;

		if (current_procedure->variable_in_symbol_list_check(*$1))
			 var_table_entry = current_procedure->get_symbol_table_entry(*$1);

		else if (program_object.variable_in_symbol_list_check(*$1))
			var_table_entry = program_object.get_symbol_table_entry(*$1);

		else
		{
			int line = get_line_number();
			report_error("Variable has not been declared", line);
		}
		////cout<<"variable "<<get_line_number()<<endl;
		$$ = new Name_Ast(*$1, var_table_entry);

		delete $1;
   */}
;

constant:
	INTEGER_NUMBER
   {/*
		Value_Type vt;
		vt.i = $1;
		$$ = new Number_Ast(vt, int_data_type);
   */}
|
	FLOAT_NUMBER
   {/*
		Value_Type vt;
		string str(*$1);
		vt.f = (float)atof(str.c_str());
		//cout<<vt.f<<endl;
		$$ = new Number_Ast(vt, float_data_type);
   */}
;
