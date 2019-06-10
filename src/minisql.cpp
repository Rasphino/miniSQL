//
// Created by rasp on 19-6-10.
//

#include "minisql.h"
#include <iostream>

// int main() {
//    MiniSQL::init();
//    std::cout << "Hello world" << std::endl;
//    return 0;
//}

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
