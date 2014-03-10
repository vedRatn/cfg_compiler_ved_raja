
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

using namespace std;

#include"local-environment.hh"
#include"error-display.hh"
#include"user-options.hh"

#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"

Basic_Block::Basic_Block(int basic_block_number, list<Ast *> & ast_list)
{
	id_number = basic_block_number;
	statement_list = ast_list;
}

Basic_Block::~Basic_Block()
{
	list<Ast *>::iterator i;
	for (i = statement_list.begin(); i != statement_list.end(); i++)
		delete (*i);
}

list < Ast *> & Basic_Block::get_statement_list(){
	return statement_list;
}


int Basic_Block::get_bb_number()
{
	return id_number;
}
void Basic_Block::print_bb(ostream & file_buffer)
{
	file_buffer << BB_SPACE << "Basic_Block " << id_number ;

	list<Ast *>::iterator i;
	for(i = statement_list.begin(); i != statement_list.end(); i++)
		(*i)->print_ast(file_buffer);
	file_buffer<<"\n";
}

Eval_Result & Basic_Block::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result * result = NULL;

	file_buffer <<"\n"<< BB_SPACE << "Basic Block: " << id_number << "\n";
	// cout<<statement_list.size()<<endl;
	list <Ast *>::iterator i;
	for (i = statement_list.begin(); i != statement_list.end(); i++)
	{
		if((*i) == NULL)
			report_error ("Ast pointer seems to be NULL", NOLINE);

		result = &((*i)->evaluate(eval_env, file_buffer)); 
	}

	return *result;
}

/* My changes */

Basic_Block * Basic_Block::get_next_bb(list < Basic_Block * > basic_block_list){
	int value;
	if(statement_list.size() == 0){
		char error[200];
		sprintf(error , "Atleast one of true, false, direct successors should be set" , value);
		string str(error);
        report_error(str, NOLINE);
	}
	value = statement_list.back()->next_bb();
	// cout<<value<<endl;

	if(value == -1){
		/*encountered return statement*/
		return NULL;
	}
	if(value == 0){
		/*continue sequential execution*/
		bool flag = false;
		list<Basic_Block *>::iterator i;
		for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
		{
			if((*i)->get_bb_number() == id_number)
			{
				flag = true;
				continue;
			}
			if (flag)
				return (*i);
		}
		char error[200];
		sprintf(error , "Atleast one of true, false, direct successors should be set" , value);
		string str(error);
        report_error(str, NOLINE);
	}else{
		list<Basic_Block *>::iterator i;
		for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
		{
			if((*i)->get_bb_number() == value)
			{
				return (*i);
			}
		}
		char error[200];
		sprintf(error , "bb %d doesn't exist" , value);
		string str(error);
        report_error(str, NOLINE);
	}
}


int Basic_Block::invalidSuccessor(list < Basic_Block * > basic_block_list){
	list < int > allIds;
	for(list < Basic_Block * >::iterator it = basic_block_list.begin() ; it != basic_block_list.end() ; it++){
		allIds.push_back((*it)->id_number);
	}
	for(list < Ast * > :: iterator it = statement_list.begin() ; it != statement_list.end() ; it++){
		if((*it)->checkSuccessor(allIds) != 0){
			return (*it)->checkSuccessor(allIds);
		}
	}
	return 0;
}
