//CalcLex.h
//Charlie Ang
//November 13, 2016
//CSC 3310 Autumn 2016
//Header file including interface specifications for CalcLex 

#ifndef _CALCLEX_H
#define _CALCLEX_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <iomanip>
#include <map>
#include <algorithm>

using namespace std;

enum CalcGrammarTokens{
	EOFSY = 0, LPAREN, RPAREN, ADDOP, SUBOP, MULTOP, DIVOP, ASSIGNOP, ID, NUMCONST, READSY, WRITESY, UNKNOWNSY
};

#define YYTEXT_MAX 100
extern char yytext[YYTEXT_MAX];	//Global token text buffer 

bool yylexopen(const char filename[]);
void yytextclear();
void yytextappend();
int yylex();

bool followingCharsMatch(int c, string target);
string tokenType(int token);

int yyparse(); //for Lab 2
void match(int expectedTok);

//Nonterminal prototypes 
void program();
void stmt_list();
void stmt();
void expr();
void term_tail();
void term();
void factor_tail();
void factor();

#endif