//
// Created by rasp on 19-6-10.
//

#include "Api.h"
#include "iostream"

int Api::select(std::string& tableName) { return 1; }

int Api::select(std::string& tableName,
                std::vector<std::string>& colName,
                std::vector<std::string>& operand,
                std::vector<std::string>& cond) {
    return 1;
}

int Api::insert_record(std::string& tableName, std::vector<std::string>& value) { return 1; }

int Api::delete_record(std::string& tableName,
                       std::vector<std::string>& colName,
                       std::vector<std::string>& operand,
                       std::vector<std::string>& cond) {
    return 1;
}

bool Api::create_table(std::string& tableName,
                       std::string& primaryKey,
                       std::vector<std::string>& colName,
                       std::vector<std::string>& colType,
                       std::vector<uint8_t>& colSize,
                       std::vector<bool>& colUnique) {
    //
    // create table for catalog manager
    //
    CM::table t;
    t.name = tableName;
    t.NOF = colName.size();
    int i = 0, size = 0;
    for (i = 0; i < colName.size(); ++i) {
        CM::field f;
        f.name = colName[i];
        if (colType[i] == "INT") {
            size += 4;
            f.type = INT;
            f.N = 4;
            f.isUnique = colUnique[i];
        } else if (colType[i] == "FLOAT") {
            size += 4;
            f.type = FLOAT;
            f.N = 4;
            f.isUnique = colUnique[i];
        } else if (colType[i] == "CHAR") {
            size += colSize[i];
            f.type = CHAR_N;
            f.N = colSize[i];
            f.isUnique = colUnique[i];
        } else {
            std::cerr << "ERROR: [Api::create_table] Unknown type '" << colType[i] << "'!" << std::endl;
            return false;
        }

        if (primaryKey == colName[i]) {
            t.primaryKeyID = i;
            f.isUnique = true;
        }
        t.fields.push_back(f);
    }
    t.sizePerTuple = size;
    CM::CatalogManager& catalogManager = MiniSQL::get_catalog_manager();
    if (!catalogManager.create_table(t)) return false;
    catalogManager.save();

    //
    // create table for catalog manager
    //

    //
    // create table for index manager
    //

    return true;
}

bool Api::drop_table(std::string& tableName) { return true; }

bool Api::create_index(std::string& indexName, std::string& tableName, std::string& colName) { return true; }

bool Api::drop_index(std::string& indexName) { return false; }