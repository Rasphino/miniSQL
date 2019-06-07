#include "IndexManager.h"
#include "Bplus.h"
#include <fstream>
#include <iostream>
IM::IndexManager::IndexManager() {}
void IM::IndexManager::create_index(CM::table tableInfo, CM::index indexInfo) {
    std::string fileName;
    fileName = indexInfo.onTableName + "-" + indexInfo.name;
    BPlus<int> Btree(tableInfo);
}

void IM::IndexManager::delete_index(CM::index indexInfo) {
    std::string fileName;
    fileName = indexInfo.onTableName + "-" + indexInfo.name;
    // delete fileName
}

uint32_t IM::IndexManager::search_key(CM::index indexInfo, uint32_t key) {
    std::string fileName;
    fileName = indexInfo.onTableName + "-" + indexInfo.name;
    BPlus<int> Btree(fileName);
    return uint32_t();
}

void IM::IndexManager::insert_key(CM::index indexInfo, uint32_t offset, uint32_t key) {
    std::string fileName;
    fileName = indexInfo.onTableName + "-" + indexInfo.name;
    BPlus<int> Btree(fileName);
}

void IM::IndexManager::delete_key(CM::index indexInfo, uint32_t key) {
    std::string fileName;
    fileName = indexInfo.onTableName + "-" + indexInfo.name;
    BPlus<int> Btree(fileName);
}