//
// Created by rasp on 19-6-5.
//

#include "CatalogManager.h"
#include <iostream>

CM::CatalogManager::CatalogManager() {
    std::vector<CM::field> fields;
    fields.push_back({"bookid", DataType::INT, -1, true});
    fields.push_back({"title", DataType::CHAR_N, 256});
    fields.push_back({"author", DataType::CHAR_N, 256});
    fields.push_back({"price", DataType::FLOAT});

    CM::table t{"book", 4, 0, 520, fields};

    tables.push_back(t);
}

CM::CatalogManager::~CatalogManager() {}

CM::table& CM::CatalogManager::getTable(std::string name) {
    for (auto& t : tables) {
        if (t.name == name) {
            return t;
        }
    }
    throw(std::runtime_error("CM getTable: table name not found!"));
}
