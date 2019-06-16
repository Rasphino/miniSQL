//
// Created by rasp on 19-6-10.
//

#include <algorithm>
#include <iomanip>

#include "Api.h"
#include "iostream"

int Api::select(std::string& tableName) {
    if (!table_exist_helper(tableName)) {
        return 0;
    }

    // 将buffer中所有与该table有关的数据写回
    MiniSQL::get_record_manager().get_buffer_manager().save(tableName);

    uint32_t size = MiniSQL::get_record_manager().get_table_size(tableName);
    std::vector<int> offsets;
    offsets.reserve(size);
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
    std::cout << std::setw(20) << "─────────────────────┘" << std::endl;
}

bool Api::vectorAnd(std::vector<int>& offset, std::vector<int>& tmp) {
    int SIZE = offset.size();
    std::sort(offset.begin(), offset.end());
    std::sort(tmp.begin(), tmp.end());
    if (offset.empty() || tmp.empty()) {
        offset.clear();
        return true;
    }
    int pos = 0;
    std::vector<int> ans;
    ans.clear();
    for (int i = 0; i < tmp.size(); i++) {
        while (pos < SIZE && offset[pos] < tmp[i])
            pos++;
        if (pos < SIZE && offset[pos] == tmp[i]) {
            ans.push_back(tmp[i]);
        }
    }
    offset.swap(ans);
    return true;
}

int Api::select(std::string& tableName,
                std::vector<std::string>& colName,
                std::vector<std::string>& operand,
                std::vector<std::string>& cond) {
    if (!table_exist_helper(tableName)) {
        return 0;
    }

    MiniSQL::get_record_manager().get_buffer_manager().save(tableName);

    uint32_t size = MiniSQL::get_record_manager().get_table_size(tableName);

    std::vector<int> offsets;
    for (int i = 0; i < size; ++i) {
        offsets.push_back(i);
    }

    int limit = cond.size();
    for (int i = 0; i < limit; i++) {
        std::vector<int> tmpOffsets;
        bool flag = MiniSQL::get_record_manager().get_ids_from_condition(
            tableName, colName[i], operand[i], cond[i], tmpOffsets);
        vectorAnd(offsets, tmpOffsets);
        if (!offsets.size()) break;
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

int Api::insert_record(std::string& tableName, Record& value) {
    if (!table_exist_helper(tableName)) {
        return 0;
    }

    uint32_t offset = MiniSQL::get_record_manager().insert_record(tableName, value);
    auto& indices = MiniSQL::get_catalog_manager().get_indices();
    for (auto& index : indices) {
        if (index.onTableName == tableName)
            MiniSQL::get_index_manager().insert_key(index, value[index.onFieldID], offset);
    }
    return 1;
}

int Api::delete_record(std::string& tableName,
                       std::vector<std::string>& colName,
                       std::vector<std::string>& operand,
                       std::vector<std::string>& cond) {
    if (!table_exist_helper(tableName)) {
        return 0;
    }

    MiniSQL::get_record_manager().get_buffer_manager().save(tableName);

    uint32_t size = MiniSQL::get_record_manager().get_table_size(tableName);

    std::vector<int> offsets;
    for (int i = 0; i < size; ++i) {
        offsets.push_back(i);
    }
    int limit = cond.size();
    for (int i = 0; i < limit; i++) {
        std::vector<int> tmpOffsets;
        bool flag = MiniSQL::get_record_manager().get_ids_from_condition(
            tableName, colName[i], operand[i], cond[i], tmpOffsets);
        vectorAnd(offsets, tmpOffsets);
        if (!offsets.size()) break;
    }

    MiniSQL::get_record_manager().delete_record(tableName, offsets);

    auto& indices = MiniSQL::get_catalog_manager().get_indices();

    for (auto& offset : offsets) {
        for (auto& index : indices) {
            if (index.onTableName == tableName) {
                MiniSQL::get_index_manager().delete_key(index, offset);
            }
        }
    }

    return offsets.size();
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
    //    std::string indexName = tableName + "_PK";
    //    create_index(indexName, tableName, primaryKey);

    return true;
}

bool Api::drop_table(std::string& tableName) {
    if (!table_exist_helper(tableName)) {
        return false;
    }

    MiniSQL::get_record_manager().get_buffer_manager().set_free(tableName);

    auto& indices = MiniSQL::get_catalog_manager().get_indices();
    for (auto& index : indices) {
        if (index.onTableName == tableName) {
            MiniSQL::get_index_manager().delete_index(index);
        }
    }

    return true;
}

bool Api::create_index(std::string& indexName, std::string& tableName, std::string& colName) {
    CM::index index;
    index.name = indexName;
    index.onTableName = tableName;

    if (!table_exist_helper(tableName)) {
        return false;
    }
    CM::table& table = MiniSQL::get_catalog_manager().get_table(tableName);

    int i = 0;
    for (i = 0; i < table.NOF; ++i) {
        if (table.fields[i].name == colName) break;
    }
    if (i == table.NOF) return false;

    index.onFieldID = i;
    if (!MiniSQL::get_catalog_manager().create_index(index)) return false;
    MiniSQL::get_catalog_manager().save();

    // 将buffer中所有与该table有关的数据写回
    MiniSQL::get_record_manager().get_buffer_manager().save(tableName);

    uint32_t size = MiniSQL::get_record_manager().get_table_size(tableName);
    std::vector<int> offsets;
    offsets.reserve(size);
    for (int i = 0; i < size; ++i) {
        offsets.push_back(i);
    }
    Records records;

    auto& t = MiniSQL::get_catalog_manager().get_table(tableName);
    uint32_t beginOffset = 0;
    for (int j = 0; j < i; ++j) {
        if (t.fields[j].type == DataType::INT) {
            beginOffset += 4;
        } else if (t.fields[j].type == DataType::CHAR_N) {
            beginOffset += t.fields[j].N;
        } else if (t.fields[j].type == DataType::FLOAT) {
            beginOffset += 4;
        }
    }

    if (table.fields[i].type == INT) {
        std::vector<IM::link<int>> keys;
        for (const auto& offset : offsets) {
            void* data = MiniSQL::get_record_manager().get_buffer_manager().read(tableName, offset);
            int d = *(reinterpret_cast<int*>(static_cast<char*>(data) + beginOffset));
            bool isNotDelete = *(reinterpret_cast<bool*>(static_cast<char*>(data) + table.sizePerTuple));
            if (isNotDelete) keys.emplace_back(d, offset);
        }
        return MiniSQL::get_index_manager().create_index(table, index, keys);
    } else if (table.fields[i].type == CHAR_N) {
        std::vector<IM::link<std::string>> keys;
        for (const auto& offset : offsets) {
            void* data = MiniSQL::get_record_manager().get_buffer_manager().read(tableName, offset);
            std::string d(static_cast<char*>(data) + beginOffset,
                          std::min(std::strlen(static_cast<char*>(data) + beginOffset), (size_t)t.fields[i].N));
            bool isNotDelete = *(reinterpret_cast<bool*>(static_cast<char*>(data) + table.sizePerTuple));
            if (isNotDelete) keys.emplace_back(d, offset);
        }
        return MiniSQL::get_index_manager().create_index(table, index, keys);
    } else if (table.fields[i].type == FLOAT) {
        std::vector<IM::link<float>> keys;
        for (const auto& offset : offsets) {
            void* data = MiniSQL::get_record_manager().get_buffer_manager().read(tableName, offset);
            float d = *(reinterpret_cast<float*>(static_cast<char*>(data) + beginOffset));
            bool isNotDelete = *(reinterpret_cast<bool*>(static_cast<char*>(data) + table.sizePerTuple));
            if (isNotDelete) keys.emplace_back(d, offset);
        }
        return MiniSQL::get_index_manager().create_index(table, index, keys);
    } else
        return false;
}

bool Api::drop_index(std::string& indexName) {
    try {
        MiniSQL::get_catalog_manager().get_index(indexName);
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    auto& index = MiniSQL::get_catalog_manager().get_index(indexName);
    MiniSQL::get_index_manager().delete_index(index);
    return false;
}

bool Api::table_exist_helper(std::string& tableName) {
    try {
        MiniSQL::get_catalog_manager().get_table(tableName);
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    return true;
}
