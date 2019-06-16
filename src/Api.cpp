//
// Created by rasp on 19-6-10.
//

#include <iomanip>

#include "Api.h"
#include "iostream"

int Api::select(std::string& tableName) {
    uint32_t size = MiniSQL::get_record_manager().get_table_size(tableName);
    std::vector<int> offsets;
    for (int i = 0; i < size; ++i) {
        offsets.push_back(i);
    }
    Records records;
    MiniSQL::get_record_manager().select(tableName, offsets, records);

    if (records.empty()) {
        std::cout << "Select no record" << std::endl;
        return 0;
    }

    print_helper(tableName, records);

    return records.size();
}

void Api::print_helper(std::string& tableName, const Records& records) {
    auto& table = MiniSQL::get_catalog_manager().get_table(tableName);

    int rowSize = records[0].size();
    std::cout << std::setw(20) << "┌──────────────────────┬";
    for (int i = 1; i < rowSize - 1; ++i)
        std::cout << std::setw(20) << "─────────────────────┬";
    std::cout << std::setw(20) << "─────────────────────┐" << std::endl;

    std::cout << "│ " << std::setw(20) << table.fields[0].name << " │";
    for (int i = 1; i < table.fields.size(); ++i) {
        std::cout << std::setw(20) << table.fields[i].name << " │";
    }
    std::cout << std::endl;

    std::cout << std::setw(20) << "├──────────────────────┼";
    for (int i = 1; i < rowSize - 1; ++i)
        std::cout << std::setw(20) << "─────────────────────┼";
    std::cout << std::setw(20) << "─────────────────────┤" << std::endl;

    for (const auto& row : records) {
        std::cout << "│ " << std::setw(20) << row[0] << " │";
        for (int i = 1; i < row.size(); ++i) {
            std::cout << std::setw(20) << row[i] << " │";
        }
        std::cout << std::endl;
    }
    std::cout << std::setw(20) << "└──────────────────────┴";
    for (int i = 1; i < rowSize - 1; ++i)
        std::cout << std::setw(20) << "─────────────────────┴";
    std::cout << std::setw(20) << "─────────────────────┘";
}

int Api::select(std::string& tableName,
                std::vector<std::string>& colName,
                std::vector<std::string>& operand,
                std::vector<std::string>& cond) {
    return 1;
}

int Api::insert_record(std::string& tableName, Record& value) {
    MiniSQL::get_record_manager().insert_record(tableName, value);
    return 1;
}

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
    // create table for record manager
    //
    MiniSQL::get_record_manager().create_table(tableName);

    //
    // create table for index manager
    //

    return true;
}

bool Api::drop_table(std::string& tableName) { return true; }

bool Api::create_index(std::string& indexName, std::string& tableName, std::string& colName) { return true; }

bool Api::drop_index(std::string& indexName) { return false; }