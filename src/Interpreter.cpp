//
// Created by rasp on 19-6-10.
//

#include "Interpreter.h"

bool Interpreter::isExit() const { return exiting; }

void Interpreter::execute(std::string& sql) {
    Tokenizer::get_tokens(sql, tokens, types);
    if (tokens.empty() || tokens[cur] == "QUIT" || tokens[cur] == "EXIT" || tokens[cur] == "Q") {
        exit();
    } else if (types[cur] != Tokenizer::IDENTIFIER && types[cur] != Tokenizer::END) {
        error("execute", "instruction");
    } else if (tokens[cur] == "SELECT") {
        select();
    } else if (tokens[cur] == "INSERT") {
        insert();
    } else if (tokens[cur] == "DELETE") {
        remove();
    } else if (tokens[cur] == "CREATE") {
        create();
    } else if (tokens[cur] == "DROP") {
        drop();
    } else {
        std::cerr << "ERROR: [Interpreter::execute] Unknown instruction " << tokens[cur] << std::endl;
    }
    tokens.clear();
    types.clear();
    cur = 0;
}

void Interpreter::error(std::string p, std::string exp) {
    if (cur == tokens.size()) {
        std::cout << "ERROR: [Interpreter::" << p << "] expecting " << exp << ", but get nothing" << std::endl;

    } else {
        std::cout << "ERROR: [Interpreter::" << p << "] expecting " << exp << ", but get " << tokens[cur] << std::endl;
    }
}

void Interpreter::select() {
    cur++;
    if (tokens[cur] != "*" || types[cur] != Tokenizer::SYMBOL) {
        error("select", "'*' (selecting specific column is not supported!)");
        return;
    }

    cur++;
    if (tokens[cur] != "FROM" || types[cur] != Tokenizer::IDENTIFIER) {
        error("select", "FROM");
        return;
    }

    cur++;
    if (types[cur] != Tokenizer::IDENTIFIER) {
        error("select", "table name");
        return;
    }

    std::string tableName = tokens[cur];
    std::vector<std::string> colName, operand, cond;

    cur++;
    // 无WHERE，选择全部的数据

    if (cur == tokens.size() && (tokens[cur] != ";" || types[cur] != Tokenizer::END)) {
        error("select", "';'");
        return;
    } else if (cur + 1 == tokens.size()) {
        Api::select(tableName);
        return;
    }

    if (tokens[cur] != "WHERE" || types[cur] != Tokenizer::IDENTIFIER) {
        error("select", "WHERE");
        return;
    }

    while (true) {
        cur++;
        if (types[cur] != Tokenizer::IDENTIFIER) {
            error("select", "column name");
            return;
        }
        colName.push_back(tokens[cur]);

        cur++;
        if (types[cur] != Tokenizer::OPERATOR) {
            error("select", "operator");
            return;
        }
        cond.push_back(tokens[cur]);

        cur++;
        if (types[cur] != Tokenizer::NUMBER && types[cur] != Tokenizer::STRING_SINGLE &&
            types[cur] != Tokenizer::STRING_DOUBLE) {
            error("select", "value");
            return;
        }
        operand.push_back(tokens[cur]);

        cur++;
        if (types[cur] == Tokenizer::END)
            break;
        else if (tokens[cur] != "AND" || types[cur] != Tokenizer::IDENTIFIER) {
            error("select", "'and'(Conjunctive selection is not supported)");
            return;
        }
    }

    int res = Api::select(tableName, colName, operand, cond);
    std::cout << res << " record(s) selected!" << std::endl;
}

void Interpreter::insert() {
    cur++;
    if (tokens[cur] != "INTO" || types[cur] != Tokenizer::IDENTIFIER) {
        error("insert", "INTO");
        return;
    }

    cur++;
    if (types[cur] != Tokenizer::IDENTIFIER) {
        error("insert", "table name");
        return;
    }
    std::string tableName = tokens[cur];
    Record value;

    cur++;
    if (tokens[cur] != "VALUES" || types[cur] != Tokenizer::IDENTIFIER) {
        error("insert", "VALUES");
        return;
    }

    cur++;
    if (tokens[cur] != "(" || types[cur] != Tokenizer::SYMBOL) {
        error("insert", "(");
        return;
    }

    while (true) {
        cur++;
        if (types[cur] != Tokenizer::NUMBER && types[cur] != Tokenizer::STRING_SINGLE &&
            types[cur] != Tokenizer::STRING_DOUBLE) {
            error("insert", "value");
            return;
        }
        value.push_back(tokens[cur]);

        cur++;
        if (tokens[cur] == ")" && types[cur] == Tokenizer::SYMBOL) {
            break;
        } else if (tokens[cur] != "," || types[cur] != Tokenizer::SYMBOL) {
            error("insert", "','");
            return;
        }
    }

    cur++;
    if (types[cur] != Tokenizer::END) {
        error("insert", "';'");
        return;
    }

    int res = Api::insert_record(tableName, value);
    std::cout << res << " record(s) inserted!" << std::endl;
}

void Interpreter::remove() {
    cur++;
    if (tokens[cur] != "FROM" || types[cur] != Tokenizer::IDENTIFIER) {
        error("delete", "FROM");
        return;
    }

    cur++;
    if (types[cur] != Tokenizer::IDENTIFIER) {
        error("delete", "table name");
        return;
    }
    std::string tableName = tokens[cur];
    std::vector<std::string> colName, operand, cond;

    while (true) {
        cur++;
        if (types[cur] != Tokenizer::IDENTIFIER) {
            error("select", "column name");
            return;
        }
        colName.push_back(tokens[cur]);

        cur++;
        if (types[cur] != Tokenizer::OPERATOR) {
            error("select", "operator");
            return;
        }
        cond.push_back(tokens[cur]);

        cur++;
        if (types[cur] != Tokenizer::NUMBER && types[cur] != Tokenizer::STRING_SINGLE &&
            types[cur] != Tokenizer::STRING_DOUBLE) {
            error("select", "value");
            return;
        }
        operand.push_back(tokens[cur]);

        cur++;
        if (types[cur] == Tokenizer::END)
            break;
        else if (tokens[cur] != "and" || types[cur] != Tokenizer::IDENTIFIER) {
            error("select", "'and'(Conjunctive selection is not supported)");
            return;
        }
    }

    int res = Api::delete_record(tableName, colName, operand, cond);
    std::cout << res << " record(s) deleted!" << std::endl;
}

void Interpreter::create() {
    cur++;
    if (tokens[cur] == "TABLE" && types[cur] == Tokenizer::IDENTIFIER) {
        create_table();
    } else if (tokens[cur] == "INDEX" && types[cur] == Tokenizer::IDENTIFIER) {
        create_index();
    } else {
        error("create", "'table' or 'index'");
    }
}

void Interpreter::create_table() {
    cur++;
    if (types[cur] != Tokenizer::IDENTIFIER) {
        error("create_table", "table name");
        return;
    }

    std::string tableName = tokens[cur], primaryKey;
    std::vector<std::string> colName, colType;
    std::vector<uint8_t> colSize;
    std::vector<bool> colUnique;

    cur++;
    if (tokens[cur] != "(" || types[cur] != Tokenizer::SYMBOL) {
        error("create_table", "'('");
        return;
    }

    while (true) {
        cur++;
        if (types[cur] == Tokenizer::IDENTIFIER && tokens[cur] != "PRIMARY") {
            colName.push_back(tokens[cur]);
            cur++;
            if (tokens[cur] == "CHAR" && types[cur] == Tokenizer::IDENTIFIER) {
                cur++;
                if (tokens[cur] != "(" || types[cur] != Tokenizer::SYMBOL) {
                    error("create_table", "'('");
                    return;
                }
                cur++;
                int N = std::stoi(tokens[cur]);
                if (types[cur] != Tokenizer::NUMBER || N <= 0 || N > 255) {
                    error("create_table", "1-255");
                    return;
                }
                cur++;
                if (tokens[cur] != ")" || types[cur] != Tokenizer::SYMBOL) {
                    error("create_table", "'('");
                    return;
                }

                if (tokens[cur + 1] == "UNIQUE" && types[cur + 1] == Tokenizer::IDENTIFIER) {
                    cur++;
                    colUnique.push_back(true);
                } else {
                    colUnique.push_back(false);
                }
                colType.emplace_back("CHAR");
                colSize.push_back(N);
            } else if (tokens[cur] == "INT" && types[cur] == Tokenizer::IDENTIFIER) {
                if (tokens[cur + 1] == "UNIQUE" && types[cur + 1] == Tokenizer::IDENTIFIER) {
                    cur++;
                    colUnique.push_back(true);
                } else {
                    colUnique.push_back(false);
                }
                colType.emplace_back("INT");
                colSize.push_back(4);
            } else if (tokens[cur] == "FLOAT" && types[cur] == Tokenizer::IDENTIFIER) {
                if (tokens[cur + 1] == "UNIQUE" && types[cur + 1] == Tokenizer::IDENTIFIER) {
                    cur++;
                    colUnique.push_back(true);
                } else {
                    colUnique.push_back(false);
                }
                colType.emplace_back("FLOAT");
                colSize.push_back(4);
            } else {
                error("create_table", "'CHAR' or 'INT' or 'FLOAT'");
                return;
            }
            cur++;
            if (tokens[cur] != "," || types[cur] != Tokenizer::SYMBOL) {
                error("create_table", "','");
                return;
            }
        } else if (tokens[cur] == "PRIMARY" && types[cur] == Tokenizer::IDENTIFIER) {
            cur++;
            if (tokens[cur] != "KEY" || types[cur] != Tokenizer::IDENTIFIER) {
                error("create_table", "'KEY'");
                return;
            }
            cur++;
            if (tokens[cur] != "(" || types[cur] != Tokenizer::SYMBOL) {
                error("create_table", "'('");
                return;
            }
            cur++;
            if (types[cur] != Tokenizer::IDENTIFIER) {
                error("create_table", "primary key name");
                return;
            }
            primaryKey = tokens[cur];
            cur++;
            if (tokens[cur] != ")" || types[cur] != Tokenizer::SYMBOL) {
                error("create_table", "')' (Multiple primary key is not supported!)");
                return;
            }
            cur++;
            if (tokens[cur] != ")" || types[cur] != Tokenizer::SYMBOL) {
                error("create_table", "')'");
                return;
            }
            cur++;
            if (tokens[cur] != ";" || types[cur] != Tokenizer::END) {
                error("create_table", "';'");
                return;
            } else {
                break;
            }
        } else {
            error("create_table", "'PRIMARY' or field name");
            return;
        }
    }

    bool res = Api::create_table(tableName, primaryKey, colName, colType, colSize, colUnique);
    if (res) std::cout << "Create table success" << std::endl;
}

void Interpreter::create_index() {
    cur++;
    if (types[cur] != Tokenizer::IDENTIFIER) {
        error("create_index", "index name");
        return;
    }

    std::string indexName = tokens[cur], tableName, colName;

    cur++;
    if (tokens[cur] != "ON" || types[cur] != Tokenizer::IDENTIFIER) {
        error("create_index", "ON");
        return;
    }

    cur++;
    if (types[cur] != Tokenizer::IDENTIFIER) {
        error("create_index", "table name");
        return;
    }
    tableName = tokens[cur];

    cur++;
    if (tokens[cur] != "(" || types[cur] != Tokenizer::SYMBOL) {
        error("create_index", "'(");
        return;
    }

    cur++;
    if (types[cur] != Tokenizer::IDENTIFIER) {
        error("create_index", "filed name");
        return;
    }
    colName = tokens[cur];

    cur++;
    if (tokens[cur] != ")" || types[cur] != Tokenizer::SYMBOL) {
        error("create_index", "')'");
        return;
    }

    cur++;
    if (tokens[cur] != ";" || types[cur] != Tokenizer::END) {
        error("create_index", "';'");
        return;
    }

    bool res = Api::create_index(indexName, tableName, colName);
}

void Interpreter::drop() {
    cur++;
    if (tokens[cur] == "TABLE" && types[cur] == Tokenizer::IDENTIFIER) {
        drop_table();
    } else if (tokens[cur] == "INDEX" && types[cur] == Tokenizer::IDENTIFIER) {
        drop_index();
    } else {
        error("drop", "'table' or 'index'");
    }
}

void Interpreter::drop_table() {
    cur++;
    if (types[cur] != Tokenizer::IDENTIFIER) {
        error("drop_table", "table name");
        return;
    }
    std::string tableName = tokens[cur];

    cur++;
    if (tokens[cur] != ";" || types[cur] != Tokenizer::END) {
        error("drop_table", "';'");
        return;
    }

    bool res = Api::drop_table(tableName);
}

void Interpreter::drop_index() {
    cur++;
    if (types[cur] != Tokenizer::IDENTIFIER) {
        error("drop_index", "table name");
        return;
    }
    std::string indexName = tokens[cur];

    cur++;
    if (tokens[cur] != ";" || types[cur] != Tokenizer::END) {
        error("drop_index", "';'");
        return;
    }

    bool res = Api::drop_index(indexName);
}

void Interpreter::exit() {
    std::cout << "Have a nice day :D" << std::endl;
    exiting = true;
}
