//CalcInterpMain.cpp
//Charlie Ang
//November 13, 2016
//CSC 3310 Autumn 2016
//CalcInterp Main program 

#include "CalcLex.h" //implementation file includes header file 

int main(int argc, char *argv[])
{
	//Pick up commandline input filename, if any
	if (argc > 1 && (!yylexopen(argv[1])))
	{
		cout << "Error: Cannot open input file " << argv[1] << endl;
		exit(1);
	}

	yyparse();
	return 0;
}


/*
int token;
string tokenTypeSym;
int numTokens = 0;	//keeps track of number of tokens
while ((token = yylex()) != EOFSY)		//reads through file token by token until end of file reached
{
tokenTypeSym = tokenType(token);
cout << "tok = " << setw(2) << setfill('0') << token << " " << tokenTypeSym << " " << "(" << yytext << ")" << endl; //tok = 10 READSY (read)
numTokens++;
}

//at this point, token should be EOF
if (token == EOFSY)
{
tokenTypeSym = tokenType(token);
cout << "tok = " << setw(2) << setfill('0') << token << " " << tokenTypeSym << " " << "(" << yytext << ")" << endl; //tok = 00 EOFS-$$
}

cout << endl;
cout << "Number of tokens = " << numTokens << endl;	//Number of tokens = 15
*/

/*
//Identifies what the token string value is and returns the string 
string tokenType(int token)
{
	switch (token)
	{
	case 0:
		return "EOFSY--$$";	//end of file symbol 
	case 1:
		return "LPAREN";	//left parenthesis
	case 2:
		return "RPAREN";	//right parenthesis 
	case 3:
		return "ADDOP";	//add operation
	case 4:
		return "SUBOP";	//subtract operator 
	case 5:
		return "MULTOP";	//multiply operator
	case 6:
		return "DIVOP";	//divide operator
	case 7:
		return "ASSIGNOP";	//assignment operator 
	case 8:
		return "ID";	//ID
	case 9:
		return "NUMCONST";	//numeric constant 
	case 10:
		return "READSY";	//read symbol
	case 11:
		return "WRITESY";	//write symbol 
	case 12:
		return "UNKNOWNSY";	//unknown symbol
	default:
		return "Not a valid token for Calc Grammar";
	}
}
*/
