

#include <iostream>
#include <fstream>
#include <string>

#include "scanner.h"
#include <queue>
using namespace std;

int main() {
	// reading the contents of the file
    string file_path = "input.txt";


    string content = read_file(file_path);

    queue<Token> tokens = get_tokens(content);

    write_tokens(tokens);

/*
   if (!content.empty()) {
        cout << "File contents:\n" << content;
        cout << content[3];

    }*/

    return 0;
}
