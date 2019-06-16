#pragma once
#include "BTree.h"
#include "GlobalData.h"

namespace IM {

    class IndexManager {
    public:
        template<typename T>
        bool create_index(CM::table&, CM::index&, std::vector<link<T>>&);
        bool delete_index(CM::index&);

        bool insert_key(CM::index&, std::string&, int);
        bool delete_key(CM::index&, std::string&, int);
        bool delete_key(CM::index&, int);

    private:
    };

    template<typename T>
    inline bool IndexManager::create_index(CM::table& tableInfo, CM::index& indexInfo, std::vector<link<T>>& keys) {
        BTree<T>* Tree = new BTree<T>();
        int SIZE = keys.size();
        for (int i = 0; i < SIZE; i++) {
            Tree->insert_key(keys[i].first, keys[i].second);
        }
        std::string fileName = indexInfo.onTableName + "-" + indexInfo.name;

        std::ofstream OF;
        OF.open(fileName);
        Tree->print(OF);
        OF.close();
        // create file
        return false;
    }

    inline bool IndexManager::delete_index(CM::index& indexInfo) {
        std::string fileName = indexInfo.onTableName + "-" + indexInfo.name;
        std::remove(fileName.c_str());
        // delete file
        return false;
    }

    inline bool IndexManager::insert_key(CM::index& indexInfo, std::string& _key, int id) {
        std::string fileName = indexInfo.onTableName + "-" + indexInfo.name;

        int type;
        std::ifstream TY;
        TY.open(fileName);
        TY >> type;
        TY.close();
        if (type == 0) {
            BTree<int>*P = new BTree<int>();
            P->build(fileName);
            int key = std::stoi(_key);
            P->insert_key(key, id);
            std::ofstream OF;
            OF.open(fileName);
            P->print(OF);
            OF.close();
        } else if (type == 1) {
            BTree<float>* P = new BTree<float>();
            P->build(fileName);
            float key = std::stof(_key);
            P->insert_key(key, id);
            std::ofstream OF;
            OF.open(fileName);
            P->print(OF);
            OF.close();
        } else if (type == 2) {
            BTree<std::string>* P = new BTree<std::string>();
            P->build(fileName);
            P->insert_key(_key, id);
            std::ofstream OF;
            OF.open(fileName);
            P->print(OF);
            OF.close();
        } else {
            return false;
		}
        return true;
    }

    inline bool IndexManager::delete_key(CM::index& indexInfo, std::string& _key, int id) {
        std::string fileName = indexInfo.onTableName + "-" + indexInfo.name;

        int type;
        std::ifstream TY;
        TY.open(fileName);
        TY >> type;
        TY.close();
        if (type == 0) {
            BTree<int>*P = new BTree<int>();
            P->build(fileName);
            int key = std::stoi(_key);
            P->Delete(key, id);
            std::ofstream OF;
            OF.open(fileName);
            P->print(OF);
            OF.close();
        } else if (type == 1) {
            BTree<float>* P = new BTree<float>();
            P->build(fileName);
            float key = std::stof(_key);
            P->Delete(key, id);
            std::ofstream OF;
            OF.open(fileName);
            P->print(OF);
            OF.close();
        } else if (type == 2) {
            BTree<std::string>* P = new BTree<std::string>();
            P->build(fileName);
            P->Delete(_key, id);
            std::ofstream OF;
            OF.open(fileName);
            P->print(OF);
            OF.close();
        } else {
            return false;
        }
        return true;
    }

    inline bool IndexManager::delete_key(CM::index&, int) { return true; }

}
