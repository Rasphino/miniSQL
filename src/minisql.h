//
// Created by rasp on 19-6-10.
//

#ifndef DB_MINISQL_MINISQL_H
#define DB_MINISQL_MINISQL_H

#include "BufferManager.h"
#include "CatalogManager.h"
//#include "RecordManager.h"
//#include "IndexManager.h"

class MiniSQL {
public:
    static void init();
    static void clean_up();
    static BM::BufferManager& get_buffer_manager();
    static CM::CatalogManager& get_catalog_manager();
    //    static RM::RecordManager& get_record_manager();
    //    static IM::IndexManager& get_index_manager();

private:
    static BM::BufferManager* bufferManager;
    static CM::CatalogManager* catalogManager;
    //    static RM::RecordManager* recordManager;
    //    static IM::IndexManager* indexManager;
};

#endif // DB_MINISQL_MINISQL_H
