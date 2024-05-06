#include <iostream>
#include <string>
#include <fstream>
#include <queue>

#include "scanner.h"

using namespace std;


string read_file(const string& path) {
    string text{""};
    string line{""};

    ifstream input_file;
    input_file.open(path);

    while(!input_file.eof()){
        getline(input_file, line);
        text += line + "\n";
    }

    return text;
}

void write_tokens(queue<Token> tokens){

    ofstream outFile;
    outFile.open("tokens.txt");

    if (!outFile.is_open()) {
        cerr << "Error: Unable to open the output file." << endl;
        return;
    }

    while (!tokens.empty()) {
        Token token = tokens.front();
        outFile << "Value: " << token.value << ", Type: " << token.type << "\n";
        tokens.pop();
    }

    outFile.close();
}

string scanner:: isReserved (string str){
    if (str == "if") {
        return "IF";
    } else if (str == "then") {
        return "THEN";
    } else if (str == "else") {
        return "ELSE";
    } else if (str == "end") {
        return "END";
    } else if (str == "repeat") {
        return "REPEAT";
    } else if (str == "until") {
        return "UNTIL";
    } else if (str == "read") {
        return "READ";
    } else if (str == "write") {
        return "WRITE";
    } else {
        return ""; // Not a reserved keyword (empty)
    }
}


// function to get the tokens from the tiny language
std:: queue<Token> scanner:: get_tokens(string tiny){

    std::  queue<Token> scanner;
    string concatenate = "";
    Token token;
    bool isToken =false; //

    for (int i = 0; i < (int)tiny.length(); i++) {

        isToken=false;

        if (tiny[i] == '{') { // for the comment
            while (tiny[i] != '}') {
                i++; // discard it
            }
        }
        else if (isalpha(tiny[i])) {
            while (isalpha(tiny[i])) {
                concatenate += tiny[i];
                i++;
            } // when this finish it means the word is collected in the concatenate
            i--; // because the for loop will increase it by 1
            token.value = concatenate;
            // we need to check if this concatenate value us reserved word or not!!

            if (isReserved(concatenate).length()) {
                token.type = isReserved(concatenate);
            } else {
                token.type = "IDENTIFIER";
            }
            isToken=true;
        }
        else if (isdigit(tiny[i])) {
            while (isdigit(tiny[i])) {
                concatenate += tiny[i];
                i++;
            }
            i--;
            token.value = concatenate;
            token.type = "NUMBER";
            isToken=true;
        }
        else if (tiny[i] == ':' && tiny[i + 1] == '=') {
            token.value = ":=";
            token.type = "ASSIGN";
            i++;
            isToken=true;
        }
        else if (tiny[i] == '+') {
            concatenate += tiny[i];
            token.value = concatenate;
            token.type = "PLUS";
            isToken=true;
        }
        else if (tiny[i] == '-') {
            concatenate += tiny[i];
            token.value = concatenate;
            token.type = "MINUS";
            isToken=true;
        }
        else if (tiny[i] == '/') {
            concatenate += tiny[i];
            token.value = concatenate;
            token.type = "DIV";
            isToken=true;
        }
        else if (tiny[i] == '*') {
            concatenate += tiny[i];
            token.value = concatenate;
            token.type = "MULT";
            isToken=true;
        }
        else if (tiny[i] == '=') {
            concatenate += tiny[i];
            token.value = concatenate;
            token.type = "EQUAL";
            isToken=true;
        }
        else if (tiny[i] == '<') {
            concatenate += tiny[i];
            token.value = concatenate;
            token.type = "LESSTHAN";
            isToken=true;
        }
        else if (tiny[i] == '(') {
            concatenate += tiny[i];
            token.value = concatenate;
            token.type = "OPENBRACKET";
            isToken=true;
        }
        else if (tiny[i] == ')') {
            concatenate += tiny[i];
            token.value = concatenate;
            token.type = "CLOSEDBRACKET";
            isToken=true;
        }
        else if (tiny[i] == ';') {
            concatenate += tiny[i];
            token.value = concatenate;
            token.type = "SEMICOLON";
            isToken=true;
        }
        else if (!isblank(tiny[i]) && tiny[i] != '\n'){
            concatenate += tiny[i];
            token.value = concatenate;
            token.type = "UNKNOWN";
            isToken=true;
        }

        concatenate = "";
        if (isToken == true) {
            scanner.push(token);
        }
    }

    return scanner;
}




