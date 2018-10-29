//
//  main.cpp
//  Forth Lexer
//
//  Created by Alec on 10/11/18.
//  Copyright © 2018 Alec. All rights reserved.
//

#include <iostream>
#include "Lexer.hpp"

int main(int argc, const char * argv[]) {
    string inFile, outFile;
    Lexer *l;
    
    if(argc == 1) {
        cout <<"Input name of file to lex: ";
        cin >>inFile;
        l = new Lexer(inFile);
    }
    else if(argc == 2) {
        inFile = argv[1];
        l = new Lexer(inFile);
    }
    else if(argc == 3) {
        inFile = argv[1];
        outFile = argv[2];
        l = new Lexer(inFile, outFile);
    }
    else {
        cerr <<"USAGE: " <<argv[0] <<" [inputfile] [outputfile]\n";
        exit(0);
    }
    
    l->lex();
    l->lexerPrint();

    return 0;
}
