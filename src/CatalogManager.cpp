//
// Created by rasp on 19-6-5.
//

#include "CatalogManager.h"
#include <fstream>
#include <iostream>

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

using namespace rapidjson;

CM::CatalogManager::CatalogManager() {
    std::ifstream in("db.json");
    IStreamWrapper isw(in);
    d.ParseStream(isw);

    for (const auto& m : d["database"].GetObject()) {
        if (strncmp(m.name.GetString(), "tables", 5) == 0) {
            for (const auto& v : m.value.GetArray()) {
                std::cout << v.GetString() << std::endl;
                table tmp;
                tmp.name = v.GetString();
                tables.push_back(tmp);
            }
        } else if (strncmp(m.name.GetString(), "indices", 7) == 0) {
            for (const auto& v : m.value.GetArray()) {
                std::cout << v.GetString() << std::endl;
                index tmp;
                tmp.name = v.GetString();
                indices.push_back(tmp);
            }
        }
    }

    auto itrJ = d["table"].MemberBegin();
    auto itrT = tables.begin();
    while (itrJ != d["table"].MemberEnd()) {
        for (const auto& item : itrJ->value.GetObject()) {
            if (strncmp(item.name.GetString(), "NOF", 3) == 0) {
                itrT->NOF = item.value.GetInt();
            } else if (strncmp(item.name.GetString(), "primaryKeyID", 12) == 0) {
                itrT->primaryKeyID = item.value.GetInt();
                itrT->fields[itrT->primaryKeyID].isUnique = true;
            } else if (strncmp(item.name.GetString(), "uniqueIDs", 9) == 0) {
                for (const auto& v : item.value.GetArray()) {
                    itrT->fields[v.GetInt()].isUnique = true;
                }
            } else if (strncmp(item.name.GetString(), "field", 5) == 0) {
                for (const auto& v : item.value.GetArray()) {
                    field f;
                    f.name = v.GetArray()[0].GetString();
                    std::string type = v.GetArray()[1].GetString();
                    if (type == "INT") {
                        f.type = DataType::INT;
                        f.N = 4;
                    } else if (type == "CHAR_N") {
                        f.type = DataType::CHAR_N;
                        f.N = v.GetArray()[2].GetInt();
                    } else if (type == "FLOAT") {
                        f.type = DataType::FLOAT;
                        f.N = 4;
                    }
                    itrT->fields.push_back(f);
                }
            }
        }
        uint32_t sizePerTuple = 0;
        for (const auto& f : itrT->fields) {
            sizePerTuple += f.N;
        }
        itrT->sizePerTuple = sizePerTuple;
        itrJ++, itrT++;
    }

    itrJ = d["index"].MemberBegin();
    auto itrI = indices.begin();
    while (itrJ != d["index"].MemberEnd()) {
        for (const auto& item : itrJ->value.GetObject()) {
            if (strncmp(item.name.GetString(), "onTable", 7) == 0) {
                itrI->onTableName = item.value.GetString();
            } else if (strncmp(item.name.GetString(), "onFieldID", 9) == 0) {
                itrI->onFieldID = item.value.GetInt();
            }
        }
        itrJ++, itrI++;
    }
}

CM::CatalogManager::~CatalogManager() {}

CM::table& CM::CatalogManager::get_table(std::string name) {
    for (auto& t : tables) {
        if (t.name == name) {
            return t;
        }
    }
    throw(std::runtime_error("CM getTable: table name not found!"));
}

void CM::CatalogManager::save() {
    StringBuffer buffer;
    Writer<StringBuffer> writer_(buffer);
    d.Accept(writer_);
    std::clog << buffer.GetString() << std::endl;

    std::ofstream ofs("db.json");
    OStreamWrapper osw(ofs);
    Writer<OStreamWrapper> writer(osw);
    d.Accept(writer);
}

bool CM::CatalogManager::create_table(CM::table& t) {
    // 确保主键是unique的
    t.fields[t.primaryKeyID].isUnique = true;

    for (const auto& _t : tables) {
        if (_t.name == t.name) return false;
    }

    tables.push_back(t);

    auto itr = d["database"].MemberBegin();
    auto& allocator = d.GetAllocator();
    Value& _tables = (++itr)->value;
    _tables.PushBack(Value().SetString(StringRef(t.name.c_str())), allocator);

    Value& _table = d["table"];
    Value _t(kObjectType);
    Value _f(kArrayType);
    for (const auto& f : t.fields) {
        Value _tmp(kArrayType);
        _tmp.PushBack(StringRef(f.name.c_str()), allocator);
        switch (f.type) {
            case DataType::INT: {
                _tmp.PushBack(StringRef("INT"), allocator);
                break;
            }
            case DataType::CHAR_N: {
                _tmp.PushBack(StringRef("CHAR_N"), allocator);
                break;
            }
            case DataType::FLOAT: {
                _tmp.PushBack(StringRef("FLOAT"), allocator);
                break;
            }
        }

        _f.PushBack(_tmp, allocator);
    }
    Value _u(kArrayType);
    for (int i = 0; i < t.NOF; ++i) {
        if (t.fields[i].isUnique) {
            _u.PushBack(i, allocator);
        }
    }
    _t.AddMember("field", _f, allocator);
    _t.AddMember("NOF", t.NOF, allocator);
    _t.AddMember("primaryKeyID", t.primaryKeyID, allocator);
    _t.AddMember("uniqueIDs", _u, allocator);
    _table.AddMember(StringRef(t.name.c_str()), _t, allocator);

    return true;
}

bool CM::CatalogManager::create_index(CM::index& t) {
    // 检查index所在的table是否存在
    bool hasTable = false;
    for (const auto& item : tables) {
        if (item.name == t.onTableName) {
            hasTable = true;
            break;
        }
    }
    if (!hasTable) return false;

    for (const auto& _t : indices) {
        if (_t.name == t.name) return false;
    }

    indices.push_back(t);

    auto itr = d["database"].MemberBegin();
    auto& allocator = d.GetAllocator();
    ++itr;
    Value& _tables = (++itr)->value;
    _tables.PushBack(Value().SetString(StringRef(t.name.c_str())), allocator);

    Value& _index = d["index"];
    Value _i(kObjectType);
    _i.AddMember(StringRef("onTable"), StringRef(t.onTableName.c_str()), allocator);
    _i.AddMember(StringRef("onFieldID"), t.onFieldID, allocator);
    _index.AddMember(StringRef(t.name.c_str()), _i, allocator);
    return true;
}
