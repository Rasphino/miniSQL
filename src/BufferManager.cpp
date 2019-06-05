//
// Created by rasp on 19-6-5.
//

#include <fstream>
#include <iostream>

#include "BufferManager.h"

void* BM::BufferManager::read(std::string dbName, uint32_t offset) {
    // 若buffer中已经缓存过数据，则直接返回buffer中的数据
    for (int i = 0; i < BUF_NUM; ++i) {
        if (buf[i].inUse && tables[i] != nullptr && tables[i]->name == dbName && buf[i].beginOffset <= offset &&
            buf[i].endOffset >= offset) {
            buf[i].accessTimes += 1;
#ifndef NDEBUG
            std::clog << offset << " from buf" << std::endl;
#endif
            return buf[i].buf + tables[i]->sizePerTuple * (offset - buf[i].beginOffset);
        }
    }

    CM::table& t = cm.getTable(dbName);
    std::ifstream is;
    is.open(t.name, std::ios::binary);

    // 查找空余的buffer
    int i = 0;
    while (buf[i].inUse && i < BUF_NUM)
        i++;

    // 若所有buffer都已使用，则替换最近访问次数最少的buffer
    if (i == BUF_NUM) {
        int bufToBeReplace = 0;
        for (int j = 1; j < BUF_NUM; ++j) {
            if (buf[j].accessTimes < buf[bufToBeReplace].accessTimes && !buf[j].isPinned) {
                bufToBeReplace = j;
            }
        }
#ifndef NDEBUG
        std::clog << "replace " << bufToBeReplace << std::endl;
#endif
        i = bufToBeReplace;
    }

    tables[i] = &t;
    buf[i].inUse = true;
    buf[i].beginOffset = offset;
    buf[i].endOffset = offset + BLOCK_SIZE / t.sizePerTuple - 1;
#ifndef NDEBUG
    std::clog << "beginOffset: " << buf[i].beginOffset << "; endOffset: " << buf[i].endOffset << std::endl;
#endif

    uint64_t totalOffet = t.sizePerTuple * offset;
    is.ignore(totalOffet);
    is.read(buf[i].buf, BLOCK_SIZE);
    is.close();
    return buf[i].buf;
}

BM::BufferManager::BufferManager() {
    buf = new buffer[BUF_NUM];
    for (int i = 0; i < BUF_NUM; ++i) {
        tables[i] = nullptr;
        buf[i].accessTimes = 0;
        buf[i].isPinned = buf[i].isModified = buf[i].inUse = false;
    }
}

BM::BufferManager::~BufferManager() { delete[] buf; }
