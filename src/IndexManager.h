#pragma once
#include "BTree.h"
#include "GlobalData.h"

namespace IM {

    class IndexManager {
    public:
        template<typename T>
        bool create_index(CM::table, CM::index, std::vector<link<T>>);
        bool delete_index(CM::index);

        template<typename T>
        bool insert_key(CM::index, T, int);
        template<typename T>
        bool delete_key(CM::index, T, int);

    private:
    };

    template<typename T>
    inline bool IndexManager::create_index(CM::table tableInfo, CM::index indexInfo, std::vector<link<T>> keys) {
        std::string fileName = indexInfo.onTableName + "-" + indexInfo.name;
        // create file
        return false;
    }

    inline bool IndexManager::delete_index(CM::index indexInfo) {
        std::string fileName = indexInfo.onTableName + "-" + indexInfo.name;
        // delete file
        return false;
    }

    template<typename T>
    inline bool IndexManager::insert_key(CM::index indexInfo, T key, int id) {
        std::string fileName = indexInfo.onTableName + "-" + indexInfo.name;
        BTree<T> Tree = new BTree(fileName);

        bool tagInsert = Tree->insert(key, id);

        Tree->saveFile(fileName);
        delete Tree;

        return tagInsert;
    }

    template<typename T>
    inline bool IndexManager::delete_key(CM::index indexInfo, T key, int id) {
        std::string fileName = indexInfo.onTableName + "-" + indexInfo.name;
        BTree<T> Tree = new BTree(fileName);

        bool tagDelete = Tree->Delete(key, id);

        Tree->saveFile(fileName);
        delete Tree;

        return false;
    }
}
