//
// Created by rasp on 19-6-5.
//

#ifndef DB_MINISQL_CATALOGMANAGER_H
#define DB_MINISQL_CATALOGMANAGER_H

#include "GlobalData.h"
#include <vector>

namespace CM {
    class CatalogManager {
    public:
        CatalogManager();
        ~CatalogManager();

        table& getTable(std::string name);

    private:
        std::vector<table> tables;
        std::vector<index> indices;
    };

}

#endif // DB_MINISQL_CATALOGMANAGER_H
