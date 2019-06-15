//
// Created by rasp on 19-6-10.
//

#ifndef DB_MINISQL_TOKENIZER_H
#define DB_MINISQL_TOKENIZER_H

#include <string>
#include <vector>

class Tokenizer {
public:
    enum STATE { INVALID = -1, IDLE, END, IDENTIFIER, NUMBER, STRING_SINGLE, STRING_DOUBLE, SYMBOL, OPERATOR };

    static int get_tokens(std::string& sql, std::vector<std::string>& tokens, std::vector<int>& type);
};

#endif // DB_MINISQL_TOKENIZER_H
