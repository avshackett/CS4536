//
//  Lexer.cpp
//  Forth Lexer
//
//  Created by Alec on 10/11/18.
//  Copyright © 2018 Alec. All rights reserved.
//

#include "Lexer.hpp"

Lexer::Lexer(string inFile) : fIn(inFile), cOut(cout), fOut(NULL) {
    if(!fIn.is_open()) {
        cerr <<"Failed to open " + inFile + " for input!\n";
        exit(0);
    }
    cOut <<"------------------------------------------------------------\n"
    <<"Alec Shackett - Lab 3:" <<inFile <<"\n"
    <<"------------------------------------------------------------\n"
    <<"COMMENTS\n"
    <<"------------------------------------------------------------\n";
}

Lexer::Lexer(string inFile, string outFile) : fIn(inFile), fOut(outFile), cOut(fOut) {
    if(!fIn.is_open()) {
        cerr <<"Failed to open " + inFile + " for input!\n";
        exit(0);
    }
    cOut <<"------------------------------------------------------------\n"
    <<"Alec Shackett - Lab 3:" <<inFile <<"\n"
    <<"------------------------------------------------------------\n"
    <<"COMMENTS\n"
    <<"------------------------------------------------------------\n";
}

void Lexer::addToTable(string token, Type type) {
    SYMBOLTABLE::iterator it = dictionary.find(token);
    if (it != dictionary.end()) ++it->second.amount;
    else dictionary.insert(PAIR(token, Token(token, type)));
}

bool Lexer::notPrintable(char* token) {
    if(token == NULL) return true;
    if(!strcmp(token, "\n")) return true;
    
    for(int x = 0; x < strlen(token); ++x) {
        if((
            !isprint(token[x])) &&
            (token[x] != ' ') &&
            (token[x] != '\n') //&&
            //(token[x] != '\\')
           ) return true;
    }
    
    return false;
}

string Lexer::nextToken(const string delims) {
    string token;
    
    char* get = strtok((mounted ? NULL : stream), delims.c_str());
    mounted = true;
    
    if(notPrintable(get)) {
        readLine();
        if(fIn.eof()) {
            state = done;
            return "";
        }
        return nextToken(delims);
    }
    else {
        token = get;
        
        if(token[token.size()-1] == '\n') token.resize(token.size()-1);
        return token;
    }
}

void Lexer::readLine() {
    
    if(mounted == true) mounted = false;
    
    string temp;
    getline(fIn, temp, '\n');
    temp += '\n';
    stream = new char[temp.size()];
    strcpy(stream, temp.c_str());
}

void Lexer::lex() {
    for(;;) {
        if(fIn.eof()) break;
        switch (state) {
            case start:
                startFunc();
                break;
            case acquiringSymbol:
                acquiringSymbolFunc();
                break;
            case acquiringString:
                acquiringStringFunc();
                break;
            case foundToken:
                foundTokenFunc();
                break;
            default:
                break;
        }
    }
    fIn.close();
}

void Lexer::foundTokenFunc() {
    bool num = false;
    
    if(isnumber(cur[0])) {
        for(int x = 0; x < cur.size(); ++x)
            if(!isnumber(cur[x]))
            break;
        num = true;
    }
    
    if(num) addToTable(cur, NUMBER);
    else addToTable(cur, WORD);
    
    state = start;
}

void Lexer::acquiringStringFunc() {
    cur = nextToken("\"");
    addToTable(cur, STRING);
    state = start;
}

void Lexer::acquiringSymbolFunc() {
    if(cur[cur.size()-1] == '\n') cur.resize(cur.size()-1);
    
    if(cur == ".\"") {
        addToTable(cur, WORD);
        state = acquiringString;
    }
    else state = foundToken;
}

void Lexer::startFunc() {
    cur = nextToken(" \t");
    if(state == done) return;
    if(cur[cur.size()-1] == '\\') cOut <<nextToken("\n") <<endl;
    else if(cur[cur.size()-1] == '(') cOut <<nextToken(")") <<endl;
    else state = acquiringSymbol;
}

void Lexer::print(ostream& out) {
    out <<"------------------------------------------------------------\n"
        <<left <<"Total tokens: " <<dictionary.size() <<"\n"
        <<left <<setw(10) <<"AMOUNT" <<setw(10) <<"TYPE" <<setw(20) <<"TOKEN"
        <<"\n------------------------------------------------------------\n";
    
    SYMBOLTABLE::iterator it;
    for ( it = dictionary.begin(); it != dictionary.end(); it++ ) {
        out <<it->second;
        out <<endl;
    }
    out <<"------------------------------------------------------------\n";
    fOut.close();
}

void Token::print(ostream& out) {
    out <<left
        <<setw(10) <<amount
        <<setw(10) <<types[type]
        <<setw(50)<<"[" +token +"]";
}
