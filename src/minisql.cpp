//
// Created by rasp on 19-6-10.
//

#include "minisql.h"
#include "Api.h"
#include <iostream>

BM::BufferManager* MiniSQL::bufferManager = nullptr;
CM::CatalogManager* MiniSQL::catalogManager = nullptr;
// RM::RecordManager* MiniSQL::recordManager = nullptr;
// IM::IndexManager* MiniSQL::indexManager = nullptr;

int main() {
    MiniSQL::init();
    std::cout << "Hello world" << std::endl;
    std::vector<std::string> colName;
    std::vector<std::string> colType;
    std::vector<uint8_t> colSize;
    std::vector<bool> colUnique;
    colName.emplace_back("id"), colType.emplace_back("INT"), colSize.push_back(4), colUnique.push_back(true);
    colName.emplace_back("name"), colType.emplace_back("CHAR"), colSize.push_back(20), colUnique.push_back(false);
    colName.emplace_back("price"), colType.emplace_back("FLOAT"), colSize.push_back(4), colUnique.push_back(false);
    Api::create_table("shit", "id", colName, colType, colSize, colUnique);
    return 0;
}

void MiniSQL::init() {
    bufferManager = new BM::BufferManager;
    catalogManager = new CM::CatalogManager;
    //    recordManager = new RM::RecordManager;
    //    indexManager = IM::IndexManager;
}
void MiniSQL::clean_up() {
    delete bufferManager;
    delete catalogManager;
    //    delete recordManager;
    //    delete indexManager;
}

BM::BufferManager& MiniSQL::get_buffer_manager() { return *bufferManager; }
CM::CatalogManager& MiniSQL::get_catalog_manager() { return *catalogManager; }
