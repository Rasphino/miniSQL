//
// Created by zx on 19-6-7.
//

#ifndef DB_MINISQL_INDEXMANAGER_H
#define DB_MINISQL_INDEXMANAGER_H

#include "GlobalData.h"
#include <vector>

namespace IM {
    class IndexManager {
    public:
        IndexManager() = default;
        ~IndexManager() = default;
        void create_index(CM::table tableInfo, CM::index indexInfo);
        void delete_index(CM::index indexInfo);
        uint32_t search_key(CM::index indexInfo, uint32_t key);
        // uint32_t search_key(CM::index indexInfo, float key);
        // uint32_t search_key(CM::index indexInfo, std::string key);
        void insert_key(CM::index indexInfo, uint32_t offset, uint32_t key);
        // void insert_key(CM::index indexInfo, uint32_t offset, float key);
        // void insert_key(CM::index indexInfo, uint32_t offset, std::string key);

        void delete_key(CM::index indexInfo, uint32_t key);
        // void delete_key(CM::index indexInfo, float key);
        // void delete_key(CM::index indexInfo, std::string key);

    private:
    };

}

#endif // DB_MINISQL_IndexManager_H
