//
// Created by rasp on 19-6-5.
//

#ifndef DB_MINISQL_BUFFERMANAGER_H
#define DB_MINISQL_BUFFERMANAGER_H

#include "CatalogManager.h"
#include "GlobalData.h"
#include <string>

namespace BM {
    class BufferManager {
    public:
        BufferManager();
        ~BufferManager();

        void* read(std::string dbName, uint32_t offset);
        void save(size_t idx);
        void setModified(size_t idx);
        int append(std::string dbName, const Record& row);

    private:
        int getFreeBuffer();
        void copyToBufferHelper(const Record& row, const CM::table& t, char* p) const;

        buffer* buf;
        CM::table* tables[BUF_NUM];
        CM::CatalogManager cm;
    };
}

#endif // DB_MINISQL_BUFFERMANAGER_H
