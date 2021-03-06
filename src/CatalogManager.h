//
// Created by rasp on 19-6-5.
//

#ifndef DB_MINISQL_CATALOGMANAGER_H
#define DB_MINISQL_CATALOGMANAGER_H

#include "GlobalData.h"
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>

namespace CM {
    class CatalogManager {
    public:
        CatalogManager();
        ~CatalogManager() = default;

        table& get_table(std::string& name);
        index& get_index(std::string& name);
        std::vector<table>& get_tables();
        std::vector<index>& get_indices();
        void save();

        bool create_table(table& t, bool overWrite = false);
        bool create_index(index& t, bool overWrite = false);

        bool drop_table(std::string& tableName);
        bool drop_index(std::string& indexName);

    private:
        std::vector<table> tables;
        std::vector<index> indices;
        rapidjson::Document d;
    };

}

#endif // DB_MINISQL_CATALOGMANAGER_H
