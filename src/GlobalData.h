//
// Created by rasp on 19-6-5.
//

#ifndef DB_MINISQL_GLOBALDATA_H
#define DB_MINISQL_GLOBALDATA_H

#include <string>
#include <vector>

using Record = std::vector<std::string>;
using Records = std::vector<Record>;

const int BLOCK_SIZE = 8192;

enum DataType { INT, CHAR_N, FLOAT };

namespace CM {
    typedef struct field_ {
        std::string name;
        DataType type;
        int N = -1;
        bool isUnique = false;
    } field;

    typedef struct table_ {
        std::string name;      // 表名
        uint32_t NOF;          // field的数量
        uint32_t primaryKeyID; // 主键所在field的序号（从0开始）
        uint32_t sizePerTuple; // 每一个元组所占的空间（bytes）
        std::vector<field> fields;
    } table;

    typedef struct index_ {
        std::string name;       // 索引名
        std::string onableName; // 索引所在的表
        std::string onFieldID;  // 单值索引所在的field序号（从0开始）
    } index;

}

#endif // DB_MINISQL_GLOBALDATA_H
