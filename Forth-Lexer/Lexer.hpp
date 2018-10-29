//
//  Lexer.hpp
//  Forth Lexer
//
//  Created by Alec on 10/11/18.
//  Copyright © 2018 Alec. All rights reserved.
//

#ifndef Lexer_hpp
#define Lexer_hpp

#define SYMBOLTABLE map<string,Token>
#define PAIR(a,b) pair<string,Token>(a,b)

#include <iostream>
#include <fstream>
#include <map>
#include <iomanip>

enum Type{NUMBER, STRING, WORD};
enum State{start, acquiringSymbol, acquiringString, foundToken, done};

using namespace std;

class Token {
private:
    string types[3] = {"NUMBER", "STRING", "WORD"};
public:
    Type type;
    string token;
    int amount = 1;
    Token(string token, Type type) : token(token), type(type) {}
    void print(ostream& out);
};

class Lexer {
private:
    ifstream fIn;
    ofstream fOut;
    ostream& cOut;
    State state = start;
    SYMBOLTABLE dictionary;
    char* stream = NULL;
    string cur;
    bool mounted = false;
    
public:
    Lexer(string inFile);
    Lexer(string inFile, string outFile);
    
    void lex();
    void lexerPrint() { print(cOut); }
    void print(ostream& out);
    void startFunc();
    void acquiringSymbolFunc();
    void acquiringStringFunc();
    void foundTokenFunc();
    bool notPrintable(char* token);
    void readLine();
    void addToTable(string token, Type type);
    string nextToken(const string delims);
};

inline void operator <<( ostream& out, Token& t ) {t.print( out );}

#endif /* Lexer_hpp */
