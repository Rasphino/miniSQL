//
// Created by rasp on 19-6-15.
//

#include <algorithm>
#include <fstream>
#include <iostream>

#include <sys/stat.h>
#ifdef UNIX
#include <unistd.h>
#endif

#ifdef WIN32
#include <io.h>
#endif

#include "RecordManager.h"

RM::RecordManager::RecordManager() {
    auto& catalogManager = MiniSQL::get_catalog_manager();
    auto& t = catalogManager.get_tables();
    if (t.empty()) {
        if (access("record", 0) == -1) {
            mkdir("record", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            return;
        } else {
            for (const auto& table : t) {
                std::ifstream IF;
                IF.open("record/" + table.name);
                std::vector<uint32_t> v;
                while (!IF.eof()) {
                    uint32_t tmp;
                    IF >> tmp;
                    v.push_back(tmp);
                }
                emptySlots.emplace(table.name, v);
            }
        }
    }
}

void RM::RecordManager::create_table(std::string& tableName) {
    std::string tmp = "record/" + tableName;
    if (access(tmp.c_str(), 0) == -1) {
        std::ofstream OF(tmp);
    }
}

void RM::RecordManager::drop_table(std::string& tableName) { std::remove(std::string("record/" + tableName).c_str()); }

int RM::RecordManager::select(
    std::string& tableName, std::string& colName, std::string& operand, std::string& cond, Records& records) {
    std::vector<int> offsets;
    if (get_ids_from_condition(tableName, colName, operand, cond, offsets))
        return select(tableName, offsets);
    else
        return false;
}

int RM::RecordManager::select(std::string& tableName, std::vector<int>& offsets) { return 0; }

bool RM::RecordManager::insert_record(std::string& tableName, Record record) { return false; }

bool RM::RecordManager::delete_record(std::string& tableName,
                                      std::string& colName,
                                      std::string& operand,
                                      std::string& cond) {
    std::vector<int> offsets;
    if (get_ids_from_condition(tableName, colName, operand, cond, offsets))
        return delete_record(tableName, offsets);
    else
        return false;
}

bool RM::RecordManager::delete_record(std::string& tableName, std::vector<int>& offsets) {
    auto& emptySlot = emptySlots[tableName];
    for (const auto& offset : offsets) {
        emptySlot.push_back(offset);
        bufferManager.delete_record(tableName, offset);
    }
    return true;
}

bool RM::RecordManager::verify_data(
    void* data, uint32_t beginOffset, uint32_t endOffset, DataType type, std::string& cond, std::string& operand) {
    if (type == DataType::INT) {
        int d = *(reinterpret_cast<int*>(static_cast<char*>(data) + beginOffset));
        return cmp(d, std::stoi(operand), cond);
    } else if (type == DataType::CHAR_N) {
        std::string d(
            static_cast<char*>(data) + beginOffset,
            std::min(std::strlen(static_cast<char*>(data) + beginOffset), (unsigned long)endOffset - beginOffset));
        return cmp(d, operand, cond);
    } else if (type == DataType::FLOAT) {
        float d = *(reinterpret_cast<float*>(static_cast<char*>(data) + beginOffset));
        return cmp(d, std::stof(operand), cond);
    }
}

bool RM::RecordManager::get_ids_from_condition(
    std::string& tableName, std::string& colName, std::string& operand, std::string& cond, std::vector<int>& offsets) {
    auto& t = MiniSQL::get_catalog_manager().get_table(tableName);
    uint32_t beginOffset = 0, endOffset = 0;
    DataType type;

    for (const auto& field : t.fields) {
        if (field.name == colName) {
            endOffset = beginOffset + field.N;
            type = field.type;
            break;
        }
        beginOffset += field.N;
    }
    if (endOffset == 0) {
        std::cout << "ERROR: [RecordManager::delete_record] colName " << colName << "not found!" << std::endl;
        return false;
    }

    if (offsets.empty()) {
        for (int i = 0; i < bufferManager.get_table_size(tableName); ++i) {
            void* data = bufferManager.read(tableName, i);
            if (verify_data(data, beginOffset, endOffset, type, cond, operand)) {
                offsets.push_back(i);
            }
        }
        return true;
    } else {
        // And
        for (auto itr = offsets.begin(); itr != offsets.end(); ++itr) {
            void* data = bufferManager.read(tableName, *itr);
            if (!verify_data(data, beginOffset, endOffset, type, cond, operand)) {
                offsets.erase(itr);
            }
        }
        return true;
    }
}
