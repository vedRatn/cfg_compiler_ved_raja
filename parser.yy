
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
	pair<Data_Type, string> * decl;
	list<Ast *> * ast_list;
	Ast * ast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	Basic_Block * basic_block;
	list<Basic_Block *> * basic_block_list;
	Procedure * procedure;
};

%token <integer_value> INTEGER_NUMBER BBNUM
%token <float_value>  FLOAT_NUMBER 
%token <string_value> NAME 
%token RETURN INTEGER FLOAT DOUBLE
%token IF ELSE GOTO ASSIGN

%left ne eq
%left lt le gt ge
%left '+' '-'
%left '/' '*'

%type <symbol_table> optional_variable_declaration_list
%type <symbol_table> variable_declaration_list
%type <symbol_entry> variable_declaration
%type <decl> declaration
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
%type <ast> constant

%start program

%%

program:
	optional_declaration_list procedure_definition
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

		program_object.set_procedure_map(current_procedure, get_line_number());
		program_object.global_list_in_proc_map_check();
	}
	}
;

optional_declaration_list:
	{
	if (NOT_ONLY_PARSE)
	{
		Symbol_Table * global_table = new Symbol_Table();
		program_object.set_global_table(*global_table);
	}
	}
|
	variable_declaration_list
	{
	if (NOT_ONLY_PARSE)
	{
		Symbol_Table * global_table = $1;

		CHECK_INVARIANT((global_table != NULL), "Global declarations cannot be null");

		program_object.set_global_table(*global_table);
	}
	}
;

procedure_definition:
	NAME '(' ')'
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Procedure name cannot be null");

		string proc_name = *$1;

		current_procedure = new Procedure(void_data_type, proc_name, get_line_number());
	}
	}

	'{' optional_variable_declaration_list
	{
	if (NOT_ONLY_PARSE)
	{

		CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

		Symbol_Table * local_table = $6;

		if (local_table == NULL)
			local_table = new Symbol_Table();

		current_procedure->set_local_list(*local_table);
	}
	}

	basic_block_list '}'
	{
	if (NOT_ONLY_PARSE)
	{
		list<Basic_Block *> * bb_list = $8;

		CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");
		CHECK_INVARIANT((bb_list != NULL), "Basic block list cannot be null");

		current_procedure->set_basic_block_list(*bb_list);
		CHECK_INVARIANT((program_object.max_bb_call <= program_object.max_bb), "Basic block call invalid");
	}
	}
;

optional_variable_declaration_list:
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = NULL;
	}
	}
|
	variable_declaration_list
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Declaration statement list cannot be null here");

		$$ = $1;
	}
	}
;

variable_declaration_list:
	variable_declaration
	{
	if (NOT_ONLY_PARSE)
	{
		Symbol_Table_Entry * decl_stmt = $1;

		CHECK_INVARIANT((decl_stmt != NULL), "Non-terminal declaration statement cannot be null");

		string decl_name = decl_stmt->get_variable_name();
		CHECK_INPUT ((program_object.variable_in_proc_map_check(decl_name) == false),
				"Variable name cannot be same as the procedure name", get_line_number());

		if (current_procedure != NULL)
		{
			CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
				"Variable name cannot be same as procedure name", get_line_number());
		}

		Symbol_Table * decl_list = new Symbol_Table();
		decl_list->push_symbol(decl_stmt);

		$$ = decl_list;
	}
	}
|
	variable_declaration_list variable_declaration
	{
	if (NOT_ONLY_PARSE)
	{
		// if declaration is local then no need to check in global list
		// if declaration is global then this list is global list

		Symbol_Table_Entry * decl_stmt = $2;
		Symbol_Table * decl_list = $1;

		CHECK_INVARIANT((decl_stmt != NULL), "The declaration statement cannot be null");
		CHECK_INVARIANT((decl_list != NULL), "The declaration statement list cannot be null");

		string decl_name = decl_stmt->get_variable_name();
		CHECK_INPUT((program_object.variable_in_proc_map_check(decl_name) == false),
			"Procedure name cannot be same as the variable name", get_line_number());
		if (current_procedure != NULL)
		{
			CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
				"Variable name cannot be same as procedure name", get_line_number());
		}

		CHECK_INPUT((decl_list->variable_in_symbol_list_check(decl_name) == false), 
				"Variable is declared twice", get_line_number());

		decl_list->push_symbol(decl_stmt);
		$$ = decl_list;
	}
	}
;

variable_declaration:
	declaration ';'
	{
	if (NOT_ONLY_PARSE)
	{
		pair<Data_Type, string> * decl_stmt = $1;

		CHECK_INVARIANT((decl_stmt != NULL), "Declaration cannot be null");

		Data_Type type = decl_stmt->first;
		string decl_name = decl_stmt->second;

		Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(decl_name, type, get_line_number());

		$$ = decl_entry;

	}
	}
;

declaration:
	INTEGER NAME
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Name cannot be null");

		string name = *$2;
		Data_Type type = int_data_type;

		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);

		$$ = declar;
	}
	}
|
	FLOAT NAME 

	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Name cannot be null");

		string name = *$2;
		Data_Type type = float_data_type;

		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);

		$$ = declar;
	}
	}
|
	DOUBLE NAME 
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Name cannot be null");

		string name = *$2;
		Data_Type type = float_data_type;

		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);

		$$ = declar;
	}
	}
;

basic_block_list:
	basic_block_list basic_block
	{
	if (NOT_ONLY_PARSE)
	{
		list<Basic_Block *> * bb_list = $1;
		Basic_Block * bb = $2;

		CHECK_INVARIANT((bb_list != NULL), "New basic block cannot be null");
		CHECK_INVARIANT((bb != NULL), "Basic block cannot be null");

		bb_strictly_increasing_order_check(bb_list, bb->get_bb_number());

		bb_list->push_back($2);
		$$ = bb_list;
	}
	}
|
	basic_block
	{
	if (NOT_ONLY_PARSE)
	{
		Basic_Block * bb = $1;

		CHECK_INVARIANT((bb != NULL), "Basic block cannot be null");

		list<Basic_Block *> * bb_list = new list<Basic_Block *>;
		bb_list->push_back(bb);

		$$ = bb_list;
	}
	}
;

basic_block:
	BBNUM ':' executable_statement_list
	{
	if (NOT_ONLY_PARSE)
	{
		int bb_number = $1;
		list<Ast *> * exe_stmt = $3;

		if(bb_number > program_object.max_bb)
			program_object.max_bb = bb_number;

		Basic_Block * bb = new Basic_Block(bb_number, get_line_number());

		if (exe_stmt != NULL)
			bb->set_ast_list(*exe_stmt);
		else
		{
			list<Ast *> * ast_list = new list<Ast *>;
			bb->set_ast_list(*ast_list);
		}

		$$ = bb;
	}
	}
;

executable_statement_list:
	assignment_statement_list
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = $1;
	}
	}
|
	assignment_statement_list RETURN ';'
	{
	if (NOT_ONLY_PARSE)
	{
		list<Ast *> * assign_list = $1;
		Ast * ret = new Return_Ast(get_line_number());
		list<Ast *> * exe_list = NULL;

		if (assign_list)
			exe_list = assign_list;

		else
			exe_list = new list<Ast *>;

		exe_list->push_back(ret);

		$$ = exe_list;
	}
	}
|
	assignment_statement_list if_statement
	{
		if (NOT_ONLY_PARSE)
		{
			if($1 != NULL)
				$$ = $1;
			else
				$$ = new list < Ast * >;

			$$->push_back($2);
		}
	}
|
	assignment_statement_list goto_statement
	{
		if (NOT_ONLY_PARSE)
		{
			if($1 != NULL)
				$$ = $1;
			else
				$$ = new list <Ast *>;

			$$->push_back($2);
		}
	}

;

assignment_statement_list:
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = NULL;
	}
	}
|
	assignment_statement_list assignment_statement
	{
	if (NOT_ONLY_PARSE)
	{
		list<Ast *> * assign_list = $1;
		Ast * assign_stmt = $2;
		list<Ast *> * assign_list_new = NULL;

		CHECK_INVARIANT((assign_stmt != NULL), "Assignment statement cannot be null");

		if (assign_list == NULL)
			assign_list_new = new list<Ast *>;

		else
			assign_list_new = assign_list;

		assign_list_new->push_back(assign_stmt);

		$$ = assign_list_new;
	}
	}
;

if_statement:
	IF '(' relational_statement ')' goto_statement ELSE goto_statement
	{
		if (NOT_ONLY_PARSE)
		{
			$$ = new If_Else_Ast($3, $5, $7);
		}
	}
;

goto_statement:
	GOTO BBNUM ';'
	{
		if (NOT_ONLY_PARSE)
		{
			if($2 > program_object.max_bb_call)
				program_object.max_bb_call = $2;
			$$ = new Goto_Ast($2);
		}
	}
;

relational_statement:
	arithmetic_expression
	{
		$$ = $1;
	}
|
	relational_statement le relational_statement
	{
		if (NOT_ONLY_PARSE)
		{
			//cout<<"line3 = "<<get_line_number()<<endl;
			$$ = new Relational_Ast($1, $3, LE);
			$$->check_ast();
		}
	}
|
	relational_statement ge relational_statement
	{
		if (NOT_ONLY_PARSE)
		{
			//cout<<"line4 = "<<get_line_number()<<endl;
			$$ = new Relational_Ast($1, $3, GE);
			$$->check_ast();
		}
	}
|
	relational_statement gt relational_statement
	{
		if (NOT_ONLY_PARSE)
		{
			//cout<<"line5 = "<<get_line_number()<<endl;
			$$ = new Relational_Ast($1, $3, GT);
			$$->check_ast();
		}
	}
|
	relational_statement lt relational_statement
	{
		if (NOT_ONLY_PARSE)
		{
			//cout<<"line6 = "<<get_line_number()<<endl;
			$$ = new Relational_Ast($1, $3, LT);
			$$->check_ast();
		}
	}
|
	relational_statement eq relational_statement
	{
		if (NOT_ONLY_PARSE)
		{
			//cout<<"line7 = "<<get_line_number()<<endl;
			$$ = new Relational_Ast($1, $3, EQ);
			$$->check_ast();
		}
	}
|
	relational_statement ne relational_statement
	{
		if (NOT_ONLY_PARSE)
		{
			//cout<<"line8 = "<<get_line_number()<<endl;
			$$ = new Relational_Ast($1, $3, NE);
			$$->check_ast();
		}
	}
;


arithmetic_expression:
	constant
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = $1;
	}
	}
|
	variable
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = $1;
	}
	}
|
	'(' relational_statement ')'
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = $2;
	}
	}
|
	'(' FLOAT ')' variable
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = new Type_Cast_Ast($4, float_data_type);
	}
	}
|
	'(' INTEGER ')' variable
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = new Type_Cast_Ast($4, int_data_type);
	}
	}
|
	'(' DOUBLE ')' variable
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = new Type_Cast_Ast($4, float_data_type);
	}
	}
|
	'(' INTEGER ')' '(' arithmetic_expression ')'
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = new Type_Cast_Ast($5, int_data_type);
	}
	}
|
	'(' FLOAT ')' '(' relational_statement ')'
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = new Type_Cast_Ast($5, float_data_type);
	}
	}
|
	'(' DOUBLE ')' '(' relational_statement ')'	
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = new Type_Cast_Ast($5, float_data_type);
	}
	}
|
	'-' constant
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = new Unary_Ast($2);
		int line = get_line_number();
		$$->check_ast();
	}
	}
|
	'-' variable
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = new Unary_Ast($2);
		int line = get_line_number();
		$$->check_ast();
	}
	}
|
	'-' '(' relational_statement ')'
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = new Unary_Ast($3);
		int line = get_line_number();
		$$->check_ast();
	}
	}
|
	arithmetic_expression '+' arithmetic_expression
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = new Plus_Ast($1, $3);
		int line = get_line_number();
		$$->check_ast();
	}
	}
|
	arithmetic_expression '-' arithmetic_expression
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = new Minus_Ast($1, $3);
		int line = get_line_number();
		$$->check_ast();
	}
	}
|
	arithmetic_expression '*' arithmetic_expression
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = new Multiplication_Ast($1, $3);
		int line = get_line_number();
		$$->check_ast();
	}
	}
|
	arithmetic_expression '/' arithmetic_expression
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = new Division_Ast($1, $3);
		int line = get_line_number();
		$$->check_ast();
	}
	}
;


assignment_statement:
	variable ASSIGN relational_statement ';'
	{
	if (NOT_ONLY_PARSE)
	{
		//cout << typeid(*$3).name() << endl;
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * lhs = $1;
		Ast * rhs = $3;

		Ast * assign = new Assignment_Ast(lhs, rhs, get_line_number());

		assign->check_ast();

		$$ = assign;
	}
	}
;

variable:
	NAME
	{
	if (NOT_ONLY_PARSE)
	{
		Symbol_Table_Entry * var_table_entry;

		CHECK_INVARIANT(($1 != NULL), "Variable name cannot be null");

		string var_name = *$1;

		if (current_procedure->variable_in_symbol_list_check(var_name) == true)
			 var_table_entry = &(current_procedure->get_symbol_table_entry(var_name));

		else if (program_object.variable_in_symbol_list_check(var_name) == true)
			var_table_entry = &(program_object.get_symbol_table_entry(var_name));

		else
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Variable has not been declared");

		Ast * name_ast = new Name_Ast(var_name, *var_table_entry, get_line_number());

		$$ = name_ast;
	}
	}
;

constant:
	INTEGER_NUMBER
	{
	if (NOT_ONLY_PARSE)
	{
		Value_Type vt;
		vt.i = $1;

		Ast * num_ast = new Number_Ast(vt, int_data_type, get_line_number());

		$$ = num_ast;
	}
	}
|
	FLOAT_NUMBER
	{
	if (NOT_ONLY_PARSE)
	{
		Value_Type vt;
		vt.f = $1;
		//cout<<vt.f<<endl;
		$$ = new Number_Ast(vt, float_data_type, get_line_number());
	}
	}
;
