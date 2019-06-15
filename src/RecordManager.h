//
// Created by rasp on 19-6-15.
//

#ifndef DB_MINISQL_RECORDMANAGER_H
#define DB_MINISQL_RECORDMANAGER_H

#include <map>
#include <string>
#include <vector>

#include "BufferManager.h"
#include "GlobalData.h"
#include "minisql.h"

namespace RM {
    class RecordManager {
    public:
        RecordManager();
        ~RecordManager() = default;

        void create_table(std::string& tableName);
        void drop_table(std::string& tableName);

        int select(
            std::string& tableName, std::string& colName, std::string& operand, std::string& cond, Records& records);
        int select(std::string& tableName, std::vector<int>& offsets);

        bool insert_record(std::string& tableName, Record record);

        bool delete_record(std::string& tableName, std::string& colName, std::string& operand, std::string& cond);
        bool delete_record(std::string& tableName, std::vector<int>& offsets);

        bool get_ids_from_condition(std::string& tableName,
                                    std::string& colName,
                                    std::string& operand,
                                    std::string& cond,
                                    std::vector<int>& offsets);

    private:
        std::map<std::string, std::vector<uint32_t>> emptySlots;
        BM::BufferManager bufferManager;

        bool verify_data(void* data,
                         uint32_t beginOffset,
                         uint32_t endOffset,
                         DataType type,
                         std::string& cond,
                         std::string& operand);

        template<typename T>
        bool cmp(T a, T b, std::string& op);
    };
}

template<typename T>
bool RM::RecordManager::cmp(T a, T b, std::string& op) {
    if (op == "=")
        return a == b;
    else if (op == "<>")
        return a != b;
    else if (op == "<")
        return a < b;
    else if (op == ">")
        return a > b;
    else if (op == "<=")
        return a <= b;
    else if (op == ">=")
        return a >= b;
    else
        return false;
};

#endif // DB_MINISQL_RECORDMANAGER_H
