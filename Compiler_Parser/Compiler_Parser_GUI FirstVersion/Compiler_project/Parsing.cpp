/*
 * Parsing.cpp
 *
 *  Created on: Dec 12, 2023
 *      Author: user
 */


#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <queue>
#include "scanner.h"
#include <vector>

using namespace std;

//enum class TokenType {
//	SEMICOLON, IF, THEN, ELSE, END, REPEAT, UNTIL, ASSIGN, READ, WRITE,
//	LESS_THAN, EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, LEFT_PAREN, RIGHT_PAREN, NUMBER, IDENTIFIER, END_OF_FILE
//};

class Node
{
public:
    // identifier name, using when node is assign or read
    // to detect which variable is assigned or read
    Token id_op_label;
    Node* neighbour = NULL;

    Node(Token tok) : token(tok) {}
    Node(string nodeType, string nodeValue){
        token.type = nodeType;
        token.value = nodeValue;
    }
    Node(string nodeType){
        token.type = nodeType;
    }

    void addChild(Node* child) {
        children.push_back(child);
    }

    void printTree(int depth = 0) {
        for (int i = 0; i < depth; ++i) {
            cout << "   ";
        }
        cout << token.type;
        if (!token.value.empty()) {
            cout << " (" << token.value << ")";
        }
        if(!id_op_label.value.empty()){
            cout << " {" << id_op_label.value << "}";
        }
        cout << endl;

        for (Node* child : children) {
            child->printTree(depth + 1);
        }
        if(neighbour != NULL)
            neighbour->printTree(depth);
    }

private:
    Token token;
    vector<Node*> children;

};

class Parser
{
public:
    Parser(queue<Token> tokensQ){
        //		vector<Node*> tokensV;
        while (!tokensQ.empty())
        {
            tokens.push_back(tokensQ.front());
            tokensQ.pop();
        }
        //		tokens = tokensV;
        currentTokenIndex = 0;
    }
    Parser(vector<Token>& tokens) : tokens(tokens), currentTokenIndex(0) {}

    Node* parse() {
        return program();
    }

    void printVector(){
        for (auto t : tokens){
            cout << t.type << ", " << t.value << "\n";
        }
    }
private:
    vector<Token> tokens;
    size_t currentTokenIndex;

    Token getCurrentToken() {
        if (currentTokenIndex < tokens.size()) {
            return tokens[currentTokenIndex];
        } else {
            Token end;
            end.type = "END_OF_FILE";
            return end;
        }
    }
    Token match(string expectedType) {
        Token currentToken = getCurrentToken();
        if (currentToken.type.compare(expectedType) == 0) {
            currentTokenIndex++;
            return currentToken;
        } else {
            throw std::runtime_error("Syntax error: Expected " + expectedType + ", found " + currentToken.type + "{ "+ currentToken.value + " }" );
            //			exit(EXIT_FAILURE);
        }
    }

    string getTokenTypeString(string type) {
        if(type.compare("SEMICOLON") == 0)
            return ";";
        else if(type.compare("IF") == 0)
            return "if";
        else if(type.compare("THEN") == 0)
            return "then";
        else
            return "UNKNOWN";
    }

    Node* program() {
        return stmtSequence() ;
    }

    Node* stmtSequence() {

        Node* start= statement() ;
        Node* temp= start ;
        while (getCurrentToken().type.compare("SEMICOLON")==0) {
            match("SEMICOLON");
            Node* newtemp =statement();
            temp->neighbour = newtemp;
            temp = newtemp;
        }
        return start ;
    }

    Node* statement() {
        string cur = getCurrentToken().type;
        if (cur.compare("IF") == 0)
            return ifStmt();
        else if(cur.compare("REPEAT") == 0)
            return repeatStmt();
        else if (cur.compare("IDENTIFIER")== 0)
            return assignStmt();

        else if (cur.compare("READ")== 0)
            return readStmt();

        else if (cur.compare("WRITE")== 0)
            return writeStmt();
        else{
            cerr << "Syntax error: Unexpected token " << getCurrentToken().value << endl;
            exit(EXIT_FAILURE);
        }
    }

    Node* ifStmt() {
        match("IF");
        Token t;
        t.type = "IfStmt";
        Node* ifStmtNode = new Node(t);
        ifStmtNode->addChild(exp());
        match("THEN");
        ifStmtNode->addChild(stmtSequence());
        if (getCurrentToken().type.compare("ELSE") == 0) {
            match("ELSE");
            ifStmtNode->addChild(stmtSequence());
        }
        match("END");
        return ifStmtNode;
    }

    Node* repeatStmt() {
        match("REPEAT");
        Token t;
        t.type = "RepeatStmt";
        Node* repeatStmtNode = new Node(t);
        repeatStmtNode->addChild(stmtSequence());
        match("UNTIL");
        repeatStmtNode->addChild(exp());
        return repeatStmtNode;
    }

    Node* assignStmt() {
        Token id = match("IDENTIFIER");
        match("ASSIGN");
        Token t;
        t.type = "AssignStmt";
        t.value = getCurrentToken().value;
        Node* assignStmtNode = new Node(t);
        //        match("IDENTIFIER");
        assignStmtNode->addChild(exp());
        assignStmtNode->id_op_label = id;
        return assignStmtNode;
    }

    Node* readStmt() {
        match("READ");
        Token t;
        t.type = "ReadStmt";
        Node* readStmtNode = new Node(t);
        //		Token t2;
        //		t2.type = "Identifier";
        //		t2.value = getCurrentToken().value;
        //		readStmtNode->addChild(new Node(getCurrentToken()));
        Token id = match("IDENTIFIER");
        readStmtNode->id_op_label = id;
        return readStmtNode;
    }

    Node* writeStmt() {
        match("WRITE");
        Token t;
        t.type = "WriteStmt";
        Node* writeStmtNode = new Node(t);
        writeStmtNode->addChild(exp());
        return writeStmtNode;
    }

    Node* exp() {

        Node* left =simpleExp(); //0

        if(!getCurrentToken().type.compare("LESSTHAN") || !getCurrentToken().type.compare("EQUAL")) {
            Node* operand = new Node(getCurrentToken());

            match(getCurrentToken().type);

            Node* right = simpleExp(); //x

            operand->addChild(left) ;
            operand->addChild(right);
            left = operand;

        }
        return left ;
    }

    Node* simpleExp() {
        Node* base = term();
        Node* left = base ;
        while (!getCurrentToken().type.compare("PLUS") || !getCurrentToken().type.compare("MINUS")) {
            Node* addopnode = new Node(getCurrentToken());
            match(getCurrentToken().type);

            addopnode->addChild(left);
            Node* right = term() ;
            addopnode->addChild(right);
            left = addopnode ;
        }
        return left;
    }

    Node* term() {
        Node* base = factor();
        Node* left = base ;
        while (!getCurrentToken().type.compare("MULT") || !getCurrentToken().type.compare("DIV")) {
            Node* mulopnode = new Node(getCurrentToken());
            match(getCurrentToken().type);

            mulopnode->addChild(left);
            Node* right = factor() ;
            mulopnode->addChild(right);
            left=mulopnode ;
        }
        return left;
    }

    Node* factor() {
        Node* factorNode = nullptr;
        if (!getCurrentToken().type.compare("OPENBRACKET")) {
            match("OPENBRACKET");
            factorNode = exp();
            match("CLOSEDBRACKET");
        } else if (!getCurrentToken().type.compare("NUMBER") || !getCurrentToken().type.compare("IDENTIFIER")) {
            factorNode = new Node(getCurrentToken());
            match(getCurrentToken().type);
        } else {
            throw std::runtime_error("Syntax error: Unexpected token " + getCurrentToken().value + "{ " + getCurrentToken().type + " }");
            //			exit(EXIT_FAILURE);
        }
        return factorNode;
    }

};

