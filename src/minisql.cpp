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

void test() {
    std::vector<std::string> colName;
    std::vector<std::string> colType;
    std::vector<uint8_t> colSize;
    std::vector<bool> colUnique;
    colName.emplace_back("id"), colType.emplace_back("INT"), colSize.push_back(4), colUnique.push_back(true);
    colName.emplace_back("name"), colType.emplace_back("CHAR"), colSize.push_back(20), colUnique.push_back(false);
    colName.emplace_back("price"), colType.emplace_back("FLOAT"), colSize.push_back(4), colUnique.push_back(false);
    //    Api::create_table("shit", "id", colName, colType, colSize, colUnique);
}

int main() {
    MiniSQL::init();
    Interpreter interpreter;

    std::string sql;
    while (!interpreter.isExit()) {
        std::cout << "\nminisql $ ";
        getline(std::cin, sql);
        interpreter.execute(sql);
    }

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