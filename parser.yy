
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
	float float_value;
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
%token <float_value> FLOAT_NUMBER
%token <string_value> NAME
%token RETURN INTEGER FLOAT DOUBLE
%token IF ELSE GOTO ASSIGN_OP
%left ne eq
%left lt le gt ge
%left '+' '-'
%left '/'
%left '*'


%type <symbol_table> declaration_statement_list
%type <symbol_entry> declaration_statement
%type <basic_block_list> basic_block_list
%type <basic_block> basic_block
%type <ast_list> executable_statement_list
%type <ast_list> assignment_statement_list
%type <ast>	if_statement
%type <ast> goto_statement
%type <ast> assignment_statement
%type <ast> relational_statement
%type <ast> variable
%type <ast> constant

%start program

%%

program:
	declaration_statement_list procedure_name
	{
		program_object.set_global_table(*$1);
		return_statement_used_flag = false;				// No return statement in the current procedure till now
	}
	procedure_body
	{

		program_object.set_procedure_map(*current_procedure);

		if ($1)
			$1->global_list_in_proc_map_check(get_line_number());

		delete $1;
	}
|
	procedure_name
	{
		return_statement_used_flag = false;				// No return statement in the current procedure till now
	}
	procedure_body
	{
		program_object.set_procedure_map(*current_procedure);
	}
;

procedure_name:
	NAME '(' ')'
	{
		current_procedure = new Procedure(void_data_type, *$1);
	}
;

procedure_body:
	'{' declaration_statement_list
	{
		current_procedure->set_local_list(*$2);
		delete $2;
	}
	basic_block_list '}'
	{
		/*
		if (return_statement_used_flag == false)
		{
			int line = get_line_number();
			report_error("Atleast 1 basic block should have a return statement", line);
		}
		*/
		current_procedure->set_basic_block_list(*$4);
		if(program_object.max_bb_call > program_object.max_bb){
			char error[200];
			sprintf(error , "bb %d doesn't exist" , program_object.max_bb_call);
			string str(error);
	        report_error(str, NOLINE);
		}
		delete $4;
	}
|
	'{' basic_block_list '}'
	{
		/*
		if (return_statement_used_flag == false)
		{
			int line = get_line_number();
			report_error("Atleast 1 basic block should have a return statement", line);
		}
		*/
		//cout<<program_object.program_object.max_bb<<":"<<program_object.max_bb_call<<endl;
		current_procedure->set_basic_block_list(*$2);
		if(program_object.max_bb_call > program_object.max_bb){
			char error[200];
			sprintf(error , "bb %d doesn't exist" , program_object.max_bb_call);
			string str(error);
	        report_error(str, NOLINE);
		}
		delete $2;
	}
;

declaration_statement_list:
	declaration_statement
	{
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
	}
|
	declaration_statement_list declaration_statement
	{
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
	}
;

declaration_statement:
	INTEGER NAME ';'

	{
		$$ = new Symbol_Table_Entry(*$2, int_data_type);

		delete $2;
	}
|
	FLOAT NAME ';'

	{
		$$ = new Symbol_Table_Entry(*$2, float_data_type);

		delete $2;
	}
;

basic_block_list:
	basic_block_list basic_block
	{
		if (!$2)
		{
			int line = get_line_number();
			report_error("Basic block doesn't exist", line);
		}

		bb_strictly_increasing_order_check($$, $2->get_bb_number());

		$$ = $1;
		$$->push_back($2);
	}
|
	basic_block
	{
		if (!$1)
		{
			int line = get_line_number();
			report_error("Basic block doesn't exist", line);
		}

		$$ = new list<Basic_Block *>;
		$$->push_back($1);
	}
;

basic_block:
	BASIC_BLOCK ':' executable_statement_list
	{
		/*if (*$2 != "bb")
		{
			int line = get_line_number();
			report_error("Not basic block lable", line);
		}
		*/
	
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
	}
;

executable_statement_list:
	assignment_statement_list
	{
		$$ = $1;
	}
|
	assignment_statement_list RETURN ';'

	{
		Ast * ret = new Return_Ast();

		return_statement_used_flag = true;					// Current procedure has an occurrence of return statement

		if ($1 != NULL)
			$$ = $1;

		else
			$$ = new list<Ast *>;

		$$->push_back(ret);
	}
|
	assignment_statement_list if_statement
	{
		if($1 != NULL)
			$$ = $1;
		else
			$$ = new list < Ast * >;

		$$->push_back($2);
	}
|
	assignment_statement_list goto_statement
	{
		
		if($1 != NULL)
			$$ = $1;
		else
			$$ = new list <Ast *>;

		$$->push_back($2);
	}
;

assignment_statement_list:
	{
		$$ = NULL;
	}
|
	assignment_statement_list assignment_statement
	{
		if ($1 == NULL)
			$$ = new list<Ast *>;

		else
			$$ = $1;


		$$->push_back($2);
	}
;

if_statement:
	IF '(' relational_statement ')' goto_statement ELSE goto_statement
	{
		$$ = new If_Else_Ast($3, $5, $7);
	}
;

goto_statement:
	GOTO BASIC_BLOCK ';'

	{
		string str(*$2);
		if(atoi(str.substr(4, str.length()-1).c_str()) > program_object.max_bb_call)
			program_object.max_bb_call = atoi(str.substr(4, str.length()-1).c_str());
		$$ = new Goto_Ast(atoi(str.substr(4, str.length()-1).c_str()));
	}
;

relational_statement:



/*	variable
	{
		//cout<<"line1 = "<<get_line_number()<<endl;
		$$ = new Relational_Ast($1);
	}
|
	constant
	{
		//cout<<"line2 = "<<get_line_number()<<endl;
		$$ = new Relational_Ast($1);
	}
*/
	arithmetic_expression
	{

	}
|
	relational_statement le relational_statement
	{
		//cout<<"line3 = "<<get_line_number()<<endl;
		$$ = new Relational_Ast($1, $3, LE);
	}
|
	relational_statement ge relational_statement
	{
		//cout<<"line4 = "<<get_line_number()<<endl;
		$$ = new Relational_Ast($1, $3, GE);
	}
|
	relational_statement gt relational_statement
	{
		//cout<<"line5 = "<<get_line_number()<<endl;
		$$ = new Relational_Ast($1, $3, GT);
	}
|
	relational_statement lt relational_statement
	{
		//cout<<"line6 = "<<get_line_number()<<endl;
		$$ = new Relational_Ast($1, $3, LT);
	}
|
	relational_statement eq relational_statement
	{
		//cout<<"line7 = "<<get_line_number()<<endl;
		$$ = new Relational_Ast($1, $3, EQ);
	}
|
	relational_statement ne relational_statement
	{
		//cout<<"line8 = "<<get_line_number()<<endl;
		$$ = new Relational_Ast($1, $3, NE);
	}
;

arithmetic_expression:
	constant
	{

	}
|
	variable
	{

	}
|
	'(' relational_statement ')'
	{

	}
|
	'(' FLOAT ')' variable
	{

	}
|
	'(' INTEGER ')' variable
	{

	}
|
	'(' DOUBLE ')' variable
	{
		
	}
|
	'(' INTEGER ')' '(' arithmetic_expression ')'
	{

	}
|
	'(' FLOAT ')' '(' relational_statement ')'
	{

	}
|
	'(' DOUBLE ')' '(' relational_statement ')'	
	{

	}
|
	'-' constant
	{

	}
|
	'-' variable
	{

	}
|
	'-' '(' relational_statement ')'
	{

	}
|
	arithmetic_expression '+' arithmetic_expression
	{

	}
|
	arithmetic_expression '-' arithmetic_expression
	{
		
	}
|
	arithmetic_expression '*' arithmetic_expression
	{
		
	}
|
	arithmetic_expression '/' arithmetic_expression
	{
		
	}
;

assignment_statement:
	variable ASSIGN_OP relational_statement ';'
	{
		$$ = new Assignment_Ast($1, $3);
		int line = get_line_number();
		//cout<<"ass line = "<<line<<endl;
		$$->check_ast(line);
	}
;

variable:
	NAME
	{
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
	}
;

constant:
	INTEGER_NUMBER
	{
		$$ = new Number_Ast<int>($1, int_data_type);
	}
|
	FLOAT_NUMBER
	{
		$$ = new Number_Ast<float>($1, float_data_type);
	}
;
