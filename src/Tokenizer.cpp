//
// Created by rasp on 19-6-10.
//

#include "Tokenizer.h"
#include <iostream>
int Tokenizer::get_tokens(std::string& sql, std::vector<std::string>& tokens, std::vector<int>& type) {
    STATE state = IDLE;
    int cursor = 0;
    std::string token;

    while (true) {
        bool moveCursor = true;
        char ch = sql[cursor];
        // to upper
        if (state != STRING_DOUBLE && state != STRING_SINGLE && islower(ch)) ch = toupper(ch);
        if (state == IDLE) {
            if (ch == ';') {
                state = END;
                moveCursor = false;
            } else if (isalpha(ch) || ch == '_') {
                state = IDENTIFIER;
                moveCursor = false;
            } else if (isdigit(ch) || ch == '-' || ch == '+' || ch == '.') {
                state = NUMBER;
                moveCursor = false;
            } else if (ch == '\'') {
                state = STRING_SINGLE;
            } else if (ch == '"') {
                state = STRING_DOUBLE;
            } else if (ch == ',' || ch == '(' || ch == ')' || ch == '*') {
                state = SYMBOL;
                moveCursor = false;
            } else if (ch == '=' || ch == '<' || ch == '>') {
                state = OPERATOR;
                moveCursor = false;
            } else if (ch == ' ' || ch == '\n' || ch == '\t') {
            } else if (ch == 0) {
                break;
            } else {
                std::cerr << "ERROR: [Tokenizer::getTokens] Unknown character '" << ch << "'!" << std::endl;
                tokens.push_back(token);
                type.push_back(INVALID);
                token.clear();
            }
        } else if (state == END) {
            token += ch;
            tokens.push_back(token);
            type.push_back(END);
            token.clear();
            state = IDLE;
        } else if (state == IDENTIFIER) {
            if (isalnum(ch) || ch == '_') {
                token += ch;
            } else {
                tokens.push_back(token);
                type.push_back(IDENTIFIER);
                token.clear();
                state = IDLE;
                moveCursor = false;
            }
        } else if (state == NUMBER) {
            if (isdigit(ch) || ch == '-' || ch == '+' || ch == '.') {
                token += ch;
            } else {
                tokens.push_back(token);
                type.push_back(NUMBER);
                token.clear();
                state = IDLE;
                moveCursor = false;
            }
        } else if (state == STRING_SINGLE || state == STRING_DOUBLE) {
            if ((ch == '\'' && state == STRING_SINGLE) || (ch == '"' && state == STRING_DOUBLE)) {
                tokens.push_back(token);
                type.push_back(NUMBER);
                token.clear();
                state = IDLE;
            } else {
                token += ch;
            }
        } else if (state == SYMBOL) {
            token += ch;
            tokens.push_back(token);
            type.push_back(SYMBOL);
            token.clear();
            state = IDLE;
        } else if (state == OPERATOR) {
            if (ch == '=' || ch == '<' || ch == '>')
                token += ch;
            else {
                tokens.push_back(token);
                type.push_back(OPERATOR);
                token.clear();

                state = IDLE;
                moveCursor = false;
            }
        }

        if (moveCursor) cursor++;
    }
    return 1;
}
