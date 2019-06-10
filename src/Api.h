//
// Created by rasp on 19-6-10.
//

#ifndef DB_MINISQL_API_H
#define DB_MINISQL_API_H

#include <string>
#include <vector>

namespace Api {
    int select();

    int insert_record();

    int delete_record();

    bool create_table();

    bool drop_table(std::string& tableName);

    bool create_index();

    bool drop_index(std::string& indexName);

};

#endif // DB_MINISQL_API_H
