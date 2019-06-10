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
    struct field {
        std::string name; // field的名称
        DataType type;    // 该field的类型
        int N = -1;       // 该field类型所占的空间
        bool isUnique = false;
    };

    struct table {
        std::string name;      // 表名
        uint32_t NOF;          // field的数量
        uint32_t primaryKeyID; // 主键所在field的序号（从0开始）
        uint32_t sizePerTuple; // 每一个元组所占的空间（bytes）
        std::vector<field> fields;
    };

    struct index {
        std::string name;        // 索引名
        std::string onTableName; // 索引所在的表
        int onFieldID;           // 单值索引所在的field序号（从0开始）
    };

}

namespace BM {
    const int BUF_NUM = 2; // buffer数量

    typedef struct buffer_ {
        uint32_t beginOffset; // 该buffer包含多个tuple的最低offset
        uint32_t endOffset;
        char buf[BLOCK_SIZE];
        uint8_t accessTimes; // 访问次数
        bool isModified;     // 数据是否被修改过
        bool isPinned;       // buffer是否被锁定
        bool inUse;          // buffer是否正在被使用
    } buffer;
}

#endif // DB_MINISQL_GLOBALDATA_H
