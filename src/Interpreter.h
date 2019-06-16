//
// Created by rasp on 19-6-10.
//

#ifndef DB_MINISQL_INTERPRETER_H
#define DB_MINISQL_INTERPRETER_H

#include <iostream>

#include "Api.h"
#include "Tokenizer.h"

class Interpreter {
public:
    Interpreter() = default;
    ~Interpreter() = default;
    bool isExit() const;
    void execute(std::string& sql);

private:
    // Token vector
    std::vector<std::string> tokens;
    // Token type vector
    std::vector<int> types;
    int cur = 0;
    bool exiting = false;

    void error(std::string p, std::string exp);
    void select();
    void insert();
    void remove();
    void create();
    void create_table();
    void create_index();
    void drop();
    void drop_table();
    void drop_index();
    void exec_file();
    void exit();
};

#endif // DB_MINISQL_INTERPRETER_H
