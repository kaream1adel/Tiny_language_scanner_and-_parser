// /*
//  * Parsing.cpp
//  *
//  *  Created on: Dec 12, 2023
//  *      Author: user
//  */

// #include "parser.h"
// #include "scanner.h"


// using namespace std;

// Node::Node(Token token) : token(token) {}

// Node::Node(std::string nodeType, std::string nodeValue) {
//     token.type = nodeType;
//     token.value = nodeValue;
// }

// Node::Node(std::string nodeType) {
//     token.type = nodeType;
// }

// void Node::addChild(Node* child) {
//     children.push_back(child);
// }
// std::vector<Node*>& Node::getChildren() {
//     return children;
// }

// std::string& Node::getTokenType(){
//     return token.type;
// }

// //-------------------------------------------------


// Parser::Parser() {
//     // Initialization code, if needed
// }

// Parser::Parser(queue<Token>& tokensQ){
//     //vector<Node*> tokensV;
//     while (!tokensQ.empty())
//     {
//         tokens.push_back(tokensQ.front());
//         tokensQ.pop();
//     }
//     //		tokens = tokensV;
//     currentTokenIndex = 0;
// }
// Parser::Parser(std::vector<Token>& tokens) : tokens(tokens), currentTokenIndex(0) {}

// Node* Parser::parse() {
//     return program();
// }

// Token Parser::getCurrentToken() {
//     if (currentTokenIndex < tokens.size()) {
//         return tokens[currentTokenIndex];
//     } else {
//         Token end;
//         end.type = "END_OF_FILE";
//         return end;
//     }
// }

// Token Parser::match(std::string expectedType) {
//     Token currentToken = getCurrentToken();
//     if (currentToken.type.compare(expectedType) == 0) {
//         currentTokenIndex++;
//         return currentToken;
//     } else {
//         throw std::runtime_error("Syntax error: Expected " + expectedType + ", found " + currentToken.type + "{ " + currentToken.value + " }");
//     }
// }

// std::string Parser::getTokenTypeString(std::string type) {
//     if (type.compare("SEMICOLON") == 0)
//         return ";";
//     else if (type.compare("IF") == 0)
//         return "if";
//     else if (type.compare("THEN") == 0)
//         return "then";
//     else
//         return "UNKNOWN";
// }

// Node* Parser::program() {
//     Token t;
//     t.type = "Program";
//     Node* programNode = new Node(t);
//     programNode->addChild(stmtSequence());
//     return programNode;
// }

// Node* Parser::stmtSequence() {
//     Token t;
//     t.type = "StmtSequence";
//     Node* stmtSequenceNode = new Node(t);
//     stmtSequenceNode->addChild(statement());
//     while (getCurrentToken().type.compare("SEMICOLON") == 0) {
//         match("SEMICOLON");
//         stmtSequenceNode->addChild(statement());
//     }
//     return stmtSequenceNode;
// }

// Node* Parser::statement() {
//     std::string cur = getCurrentToken().type;
//     if (cur.compare("IF") == 0)
//         return ifStmt();
//     else if (cur.compare("REPEAT") == 0)
//         return repeatStmt();
//     else if (cur.compare("IDENTIFIER") == 0)
//         return assignStmt();
//     else if (cur.compare("READ") == 0)
//         return readStmt();
//     else if (cur.compare("WRITE") == 0)
//         return writeStmt();
//     else {
//         throw std::runtime_error("Syntax error: Unexpected token " + getCurrentToken().value);
//     }
// }

// Node* Parser::ifStmt() {
//     match("IF");
//     Token t;
//     t.type = "IfStmt";
//     Node* ifStmtNode = new Node(t);
//     ifStmtNode->addChild(exp());
//     match("THEN");
//     ifStmtNode->addChild(stmtSequence());
//     if (getCurrentToken().type.compare("ELSE") == 0) {
//         match("ELSE");
//         ifStmtNode->addChild(stmtSequence());
//     }
//     match("END");
//     return ifStmtNode;
// }

// Node* Parser::repeatStmt() {
//     match("REPEAT");
//     Token t;
//     t.type = "RepeatStmt";
//     Node* repeatStmtNode = new Node(t);
//     repeatStmtNode->addChild(stmtSequence());
//     match("UNTIL");
//     repeatStmtNode->addChild(exp());
//     return repeatStmtNode;
// }

// Node* Parser::assignStmt() {
//     Token id = match("IDENTIFIER");
//     match("ASSIGN");
//     Token t;
//     t.type = "AssignStmt";
//     t.value = getCurrentToken().value;
//     Node* assignStmtNode = new Node(t);
//     assignStmtNode->addChild(exp());
//     assignStmtNode->id_op_label = id;
//     return assignStmtNode;
// }

// Node* Parser::readStmt() {
//     match("READ");
//     Token t;
//     t.type = "ReadStmt";
//     Node* readStmtNode = new Node(t);
//     Token id = match("IDENTIFIER");
//     readStmtNode->id_op_label = id;
//     return readStmtNode;
// }

// Node* Parser::writeStmt() {
//     match("WRITE");
//     Token t;
//     t.type = "WriteStmt";
//     Node* writeStmtNode = new Node(t);
//     writeStmtNode->addChild(exp());
//     return writeStmtNode;
// }

// Node* Parser::exp() {
//     Token t;
//     t.type = "ExpStmt";
//     Node* expNode = new Node(t);
//     expNode->addChild(simpleExp());
//     if (!getCurrentToken().type.compare("LESSTHAN") || !getCurrentToken().type.compare("EQUAL")) {
//         expNode->id_op_label = getCurrentToken();
//         match(getCurrentToken().type);
//         expNode->addChild(simpleExp());
//     }
//     return expNode;
// }

// Node* Parser::simpleExp() {
//     Node* simpleExpNode = new Node("SimpleExp");
//     simpleExpNode->addChild(term());
//     while (!getCurrentToken().type.compare("PLUS") || !getCurrentToken().type.compare("MINUS")) {
//         Node* addOpNode = new Node(getCurrentToken());
//         simpleExpNode->addChild(addOpNode);
//         match(getCurrentToken().type);
//         simpleExpNode->addChild(term());
//     }
//     return simpleExpNode;
// }

// Node* Parser::term() {
//     Node* termNode = new Node("Term");
//     termNode->addChild(factor());
//     while (!getCurrentToken().type.compare("MULT") || !getCurrentToken().type.compare("DIV")) {
//         Node* mulOpNode = new Node(getCurrentToken());
//         termNode->addChild(mulOpNode);
//         match(getCurrentToken().type);
//         termNode->addChild(factor());
//     }
//     return termNode;
// }

// Node* Parser::factor() {
//     Node* factorNode = nullptr;
//     if (!getCurrentToken().type.compare("OPENBRACKET")) {
//         match("OPENBRACKET");
//         factorNode = exp();
//         match("CLOSEDBRACKET");
//     } else if (!getCurrentToken().type.compare("NUMBER") || !getCurrentToken().type.compare("IDENTIFIER")) {
//         factorNode = new Node(getCurrentToken());
//         match(getCurrentToken().type);
//     } else {
//         throw std::runtime_error("Syntax error: Unexpected token " + getCurrentToken().value);
//     }
//     return factorNode;
// }



