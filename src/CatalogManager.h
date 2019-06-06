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
        ~CatalogManager();

        table& get_table(std::string name);
        void save();

        bool create_table(table& t);
        bool create_index(index& t);

    private:
        std::vector<table> tables;
        std::vector<index> indices;
        rapidjson::Document d;
    };

}

#endif // DB_MINISQL_CATALOGMANAGER_H
