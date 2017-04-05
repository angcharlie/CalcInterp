//CalcLex.cpp
//Charlie Ang
//November 13, 2016
//CSC 3310 Autumn 2016
//Implementaiton file for CalcLex.h

#include "CalcLex.h" //implementation file includes header file 

char yytext[YYTEXT_MAX];	//Global token text buffer
static int yytextpos = 0;	//allocated statically to extend across the entire program 

ifstream fin;
bool yylexopen(const char filename[]){
	fin.open(filename, ios_base::in);
	return fin.is_open();	//true if file opened, false otherwise 
}

//resets token buffer 
void yytextclear(){
	yytextpos = 0;
	yytext[yytextpos] = 0;
}


void yytextappend(int c)
{
	if (yytextpos >= YYTEXT_MAX - 1)	//if textpos at the end of buffer
	{
		return;	//ignore char
	}
	yytext[yytextpos++] = (char)c;
	yytext[yytextpos] = 0;	//add null sentinel at end of buffer 
}

int yylex(){
	int c;

	while (1){	//infinte while loop
		yytextclear();	//clear out token buffer information

		c = fin.get();	//retrives next character in file

		//skip whitespaces since those are not part of tokens
		while (c == ' ' || c == '\t' || c == '\n')
		{
			c = fin.get();	//c is now a char and not a whitespace (which isn't part of a token)
		}

		if (c == EOF )	//if end of file is reached 
		{
			return EOFSY;
		}

		//Checking for comments /*........
		if (followingCharsMatch(c, "/*"))	//if beginning of a section of comment
		{
			//while not end of comment "*/" or end of line (since comments must fit on single line)
			while (!followingCharsMatch(c, "*/") && c != '\n')	//while end of comment isn't reached or new line hasn't been reached, skip over and ignore 
			{
				c = fin.get();	//retrieve next char 
			}
			//if this point is reached, current character is either '\n' or end of comment char "*"
			continue;	//jumps to the outside while loop (while(1)) since scanner should ignore comments
		}

		if (followingCharsMatch(c, "read"))
		{
			return READSY;
		}

		if (followingCharsMatch(c, "write"))
		{
			return WRITESY;
		}

		if (followingCharsMatch(c, ":="))
		{
			return ASSIGNOP;
		}

		yytextappend(c); // append the token to buffer 

		if (c == '+')
		{
			return ADDOP;
		}

		if (c == '-'){
			return SUBOP;
		}

		if (c == '*') {
			return MULTOP;
		}

		if (c == '/'){
			return DIVOP;
		}

		if (c == '('){
			return LPAREN;
		}

		if (c == ')')
		{
			return RPAREN;
		}


		//Recognizing IDs
		yytextclear();
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))	//must start with a letter 
		{
			while ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || (c >= '0' && c <= '9'))	//rest can be chars, _, or digits 
			{
				yytextappend(c);
				c = fin.get();
			}
			//once you reach this point, no longer an ID (letter)
			fin.unget();	//returns the non ID char back
			return ID;		//yytext should hold ID string whether it is "sum" or "A"
		}

		
		//Recognizing numbers...if current char is a number
		if (c >= '0' && c <= '9')
		{
			while (c >= '0' && c <= '9')	//while number (0-9)
			{
				yytextappend(c);	//append that number to buffer 
				c = fin.get();
			}

			if (c == '.')	//if you come across a decimal point, keep reading the rest of numbers 
			{
				yytextappend(c);	//append to buffer 
				c = fin.get();	//retrieve next char 
				while (c >= '0' && c <= '9')	//while it is a number, keep appending to buffer 
				{
					yytextappend(c);
					c = fin.get();
				}
			} //at this point, we have reached the end of numeric constants 
					
			fin.unget();	//unget char that wasn't digit 
			return NUMCONST;
		}
		
		yytextappend(c);
		return UNKNOWNSY;	//unidentified char input...ignore
	}
	return EOFSY;	//should never get here 
}

//checks to see if following characters match the inputted target...looks ahead 
bool followingCharsMatch(int c, string target)
{
	for (int i = 0; i < target.length(); i++)	//iterate through max length of target string's length 
	{
		//tolower function returns lowercase equivalent to the parameter input...since scanner is case insensitive
		if (tolower(c) == target.at(i))	//if current char is equal to first char of target string (case insensitive...so converted token to lower case)
		{
			yytextappend(c);
			c = fin.get();
		}
		else //first chars don't match 
		{
			yytextclear();	//clear token buffer 
			//unget characters that were retrieved to bring back to original state/location
			while (i != 0)	//back to original location before comparing x
			{
				fin.unget();	//put a character back 
				i--;
			}
			return false;	//since we know characters/token doesn't match 
		}
	}
	fin.unget();	//unget first char that was retrieved to check for match 
	return true;	//if this point is reached, the current buffer and target is a match
}




//LAB 2
//CalcLex.cpp
//Charlie Ang
//November 27, 2016
//CSC 3310 Autumn 2016
//Implementaiton file for Calc Interpreter/Recursive Desscent Parser

//#include "CalcLex.h" //implementation file includes header file

//CalcGrammar rules
//program		-> stmt_list $$
//stmt_list		-> stmt stmt_list
//				-> nothing
//stmt			-> id := expr
//				-> read id
//				-> write expr
//expr			-> term term_tail
//term_tail		-> add_op term term_tail --equivalent to-- + term term_tail | - term term_tail 
//				-> nothing 
//term			-> factor factor_tail
//factor_tail	-> mul_op factor factor_tail --equivalent to-- * factor factor_tail | / factor factor_tail
//				-> nothing
//factor		-> ( expr )
//				-> id
//				-> number

//add_op		-> +
//				-> -
//mul_op		-> *
//				-> /

//Current lexical token
int tok;

//Symbol Table Map
map<string, double> symbolTable;	//string id as key; double as value 

int yyparse()
{
	tok = yylex();
	program();	
	return 0;
}

void match(int expectedTok)	
{
	if (tok == expectedTok)
	{
		if (tok != EOFSY)
		{
			tok = yylex(); //grabs next yylex if match 
		}	
	}
	else
	{
		cout << "Token " << expectedTok << " expected, but instead encountered " << yytext << endl;
	}
}

//Expression stack
static double stack[100];	//array  of doubles 
static int top = 0;		//keeping track of top 

static void stackinit()
{
	top = 0;
}

static int empty()
{
	return top == 0;
}

static double pop()
{
	return stack[top--];
}

static void push(double v)
{
	stack[++top] = v;	//increment top (index in array) and assign v (value) to that index 
}


//stmt_list $$
void program()
{
	stackinit();	//Initialize the execution environment 

	stmt_list();
	match(EOFSY);	
}

//stmt stmt_list | epsilon
void stmt_list()
{
	if (tok == ID || tok == READSY || tok == WRITESY)
	{
		stmt();
		stmt_list();
	}
	//otherwise epsilon 
	
}

//id := expr | read id | write expr
void stmt()
{
	if (tok == ID)
	{
		string identifier = yytext;	//identifier name to store in map with an associated double value

		for (int i = 0; i < identifier.length(); i++)
		{
			identifier[i] = tolower(identifier[i]);	//convert identifier string to lowercase 
		}

		match(ID);	//grabs next token 
		if (tok == ASSIGNOP)
		{
			match(ASSIGNOP);	//grabs next token 
			expr();
		}

		double topStack = pop();
		cout << "Assign: " << identifier << " = " << topStack << endl;

		//Add to symbol table
		symbolTable[identifier] = topStack;

		return;
	}

	if (tok == READSY)
	{
		match(READSY);	//grabs next tok
		if (tok == ID)
		{
			cout << "Read: Enter value for " << yytext << "> ";	//user enters value here 
			
			string identifier = yytext;	//key in map 
			for (int i = 0; i < identifier.length(); i++)
			{
				identifier[i] = tolower(identifier[i]);	//convert identifier string to lowercase 
			}
			

			double IDValue;				//value in map 

			cin >> IDValue;

			push(IDValue);	//push value to stack 

			//Add to symbol table 
			symbolTable[identifier] = IDValue;	//store in symbol table map 

			match(ID);	//grabs next tok
		}		
	}

	if (tok == WRITESY)
	{
		match(WRITESY);	//grabs next tok
		expr();						//performs calcualtion and should push result on top of stack 
		double stackVal = pop();	//pop value off top of stack 
		cout << "Write : " << stackVal << endl;	//output data value to screen 
	}	
}

//term term_tail 
void expr()
{
	term();
	term_tail();
}

// + term term_tail | - term term_tail | epsilon
void term_tail()
{
	if (tok == ADDOP)	// +
	{
		match(ADDOP);	//get next token
		term();

		//Do the computation 
		double val2 = pop();
		double val1 = pop();
		push(val1 + val2);	

		term_tail();
		return;
	}

	if (tok == SUBOP)	// - 
	{
		match(SUBOP);
		term();

		//Do the computation 
		double val2 = pop();
		double val1 = pop();
		push(val1 - val2);

		term_tail();
		return;
	}
	//otherwise epsilon 
}

//factor factor_tail 
void term()
{
	factor();
	factor_tail();
}

//* factor factor_tail | / factor factor_tail | epsilon 
void factor_tail()
{
	if (tok == MULTOP)	// *
	{
		match(MULTOP);
		factor();

		//Do the computation 
		double val2 = pop(); 
		double val1 = pop();
		push(val1 * val2);

		factor_tail();
		return;
	}

	if (tok == DIVOP)	// /
	{
		match(DIVOP);
		factor();

		//Do the computation 
		double val2 = pop();
		double val1 = pop();
		if (val2 != 0)
		{
			push(val1 / val2);
		}
		else
		{
			cout << "Error: cannot divide by 0!" << endl;
			push(0);
		}
		

		factor_tail();
		return;
	}
	//otherwise epsilon 
}

//( expr ) | id | number 
void factor()
{
	if (tok == LPAREN)	// (
	{
		match(LPAREN);	//grabs next token 
		expr();
		match(RPAREN);	//grabs next token 
	}
	
	if (tok == NUMCONST)	//number 
	{
		//convert yytext to all lowercase since everything in symTable is lowercase 
		for (int i = 0; i < 100; i++)
		{
			yytext[i] = tolower(yytext[i]);	//convert the entire string to lowercase 
		}
		//string yytext should now be all lowercase 

		double val = atof(yytext);	//convert value for this token...string to double conversion
		match(NUMCONST);	//get next token 

		push(val);	// do the computation 
		return;
	}

	//grabbing ID value from map and pushing value to the stack 
	if (tok == ID)	//id
	{
		//convert yytext to all lowercase since everything in symTable is lowercase 
		for (int i = 0; i < 100; i++)
		{
			yytext[i] = tolower(yytext[i]);	//convert the entire string to lowercase 
		}
		//string yytext should now be all lowercase 

		double val = symbolTable[yytext];	//save value of identifier from map 
		match(ID);

		push(val);	//push value to top of stack 
		return;
	}
	cout << "Token ( or numconst or id expected" << endl;
}