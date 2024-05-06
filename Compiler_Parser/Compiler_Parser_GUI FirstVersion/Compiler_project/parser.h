// #ifndef PARSER_H
// #define PARSER_H
// #include <iostream>
// #include <string>
// #include <stdio.h>
// #include <fstream>
// #include <queue>
// #include "scanner.h"
// #include <vector>



// using namespace std;
// class Node {
// public:
//     Token id_op_label;

//     Node(Token tok);
//     Node(std::string nodeType, std::string nodeValue);
//     Node(std::string nodeType);

//     void addChild(Node* child);
//     std::vector<Node*>& getChildren();
//     std::string& getTokenType();

// private:
//     Token token;
//     std::vector<Node*> children;
// };

// class Parser {
// public:
//     Parser();
//     Parser(queue<Token>& tokensQ);
//     Parser(std::vector<Token>& tokens);

//     Node* parse();

//     std::vector<Token> tokens;
//     size_t currentTokenIndex;

//     Token getCurrentToken();
//     Token match(std::string expectedType);
//     std::string getTokenTypeString(std::string type);

//     Node* program();
//     Node* stmtSequence();
//     Node* statement();
//     Node* ifStmt();
//     Node* repeatStmt();
//     Node* assignStmt();
//     Node* readStmt();
//     Node* writeStmt();
//     Node* exp();
//     Node* simpleExp();
//     Node* term();
//     Node* factor();
// };


// #endif // PARSER_H
