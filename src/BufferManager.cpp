//
// Created by rasp on 19-6-5.
//

#include <fstream>
#include <iostream>

#include "BufferManager.h"

void* BM::BufferManager::read(std::string dbName, uint32_t offset) {
    CM::CatalogManager cm;
    CM::table& t = cm.getTable(dbName);

    std::ifstream is;
    is.open(t.name, std::ios::binary);

    if (!is.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
    }

    char* buf = new char[BLOCK_SIZE];

    uint64_t totalOffet = t.sizePerTuple * offset;
    is.ignore(totalOffet);

    is.read(buf, BLOCK_SIZE);
    return buf;
}

BM::BufferManager::BufferManager() {
    buf = new char*[9];
    for (int i = 0; i < 9; ++i) {
        buf[i] = new char[BLOCK_SIZE];
    }
}

BM::BufferManager::~BufferManager() {
    for (int i = 0; i < 9; ++i) {
        delete[] buf[i];
    }
    delete[] buf;
}
