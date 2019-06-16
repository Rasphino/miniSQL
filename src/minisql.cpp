//
// Created by rasp on 19-6-10.
//

#include "minisql.h"
#include "Api.h"
#include "Interpreter.h"

#include <iostream>

CM::CatalogManager* MiniSQL::catalogManager = nullptr;
RM::RecordManager* MiniSQL::recordManager = nullptr;
IM::IndexManager* MiniSQL::indexManager = nullptr;

int main() {
    MiniSQL::init();
    Interpreter interpreter;

    std::string sql;
    while (!interpreter.isExit()) {
        std::cout << "\nminisql $ ";
        if (std::cin.eof()) break;
        getline(std::cin, sql);
        interpreter.execute(sql);
    }
    std::cin.clear();
    return 0;
}

void MiniSQL::init() {
    catalogManager = new CM::CatalogManager;
    recordManager = new RM::RecordManager;
    indexManager = new IM::IndexManager;
}

void MiniSQL::clean_up() {
    delete catalogManager;
    delete recordManager;
    delete indexManager;
}

CM::CatalogManager& MiniSQL::get_catalog_manager() { return *catalogManager; }
RM::RecordManager& MiniSQL::get_record_manager() { return *recordManager; }
IM::IndexManager& MiniSQL::get_index_manager() { return *indexManager; }