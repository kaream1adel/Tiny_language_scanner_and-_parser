#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <queue>

using namespace std;

class Token {
public:
    std::string type;
    std::string value;
};

class scanner{

public:
string read_file(const string& path);

std:: queue<Token> get_tokens(string tiny);

void write_tokens(queue<Token> s);

string isReserved (string str);

};



#endif // SCANNER_H
