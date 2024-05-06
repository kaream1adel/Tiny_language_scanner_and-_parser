#ifndef PARSING_H
#define PARSING_H
#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <queue>
#include "scanner.h"
#include <vector>


using namespace std;
class Node{
public:
    Token id_op_label;

    Node* neighbour = NULL;

    Node(Token tok);
    Node(string nodeType, string nodeValue);
    Node(string nodeType);

    void addChild(Node* child);
    void printTree(int depth = 0);

private:
    Token token;
    vector<Node*> children;

};

class Parser{
public:
    Parser();
    Parser(queue<Token>& tokensQ);
    Parser(vector<Token>& tokens);

    Node* parse();

    void printVector();

    vector<Token> tokens;
    size_t currentTokenIndex;

    Token getCurrentToken();
    Token match(string expectedType);
    string getTokenTypeString(string type);

    Node* program();
    Node* stmtSequence();
    Node* statement();
    Node* ifStmt();
    Node* repeatStmt();
    Node* assignStmt();
    Node* readStmt;
    Node* writeStmt();
    Node* exp();
    Node* simpleExp();
    Node* term();
    Node* factor();
};


#endif // PARSING_H
