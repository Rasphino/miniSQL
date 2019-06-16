//
// Created by rasp on 19-6-10.
//

#ifndef DB_MINISQL_API_H
#define DB_MINISQL_API_H

#include "GlobalData.h"
#include "minisql.h"
#include <string>
#include <vector>

namespace Api {
    int select(std::string& tableName);

    int select(std::string& tableName,
               std::vector<std::string>& colName,
               std::vector<std::string>& operand,
               std::vector<std::string>& cond);

    int insert_record(std::string& tableName, Record& value);

    int delete_record(std::string& tableName,
                      std::vector<std::string>& colName,
                      std::vector<std::string>& operand,
                      std::vector<std::string>& cond);

    bool create_table(std::string& tableName,
                      std::string& primaryKey,
                      std::vector<std::string>& colName,
                      std::vector<std::string>& colType,
                      std::vector<uint8_t>& colSize,
                      std::vector<bool>& colUnique);

    bool drop_table(std::string& tableName);

    bool create_index(std::string& indexName, std::string& tableName, std::string& colName);

    bool drop_index(std::string& indexName);

    void print_helper(std::string& tableName, const Records& records);

    bool table_exist_helper(std::string& tableName);
}

#endif // DB_MINISQL_API_H
