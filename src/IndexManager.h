#pragma once
#include "BTree.h"
template<typename T>
class IndexManager {
public:
    bool create_index(table, index, vector<link<T>>);
    bool delete_index(index);


    bool insert_key(index, T, int);
    bool delete_key(index, T, int);

private:
};


template<typename T>
inline bool IndexManager<T>::create_index(table tableInfo, index indexInfo, vector<link<T>>keys) {
    std::string fileName = indexInfo.onTableName + "-" + indexInfo.name;
	//create file
    return false;
}

template<typename T>
inline bool IndexManager<T>::delete_index(index) {
    std::string fileName = indexInfo.onTableName + "-" + indexInfo.name;
	//delete file
    return false;
}

template<typename T>
inline bool IndexManager<T>::insert_key(index indexInfo, T key, int id) {
    std::string fileName = indexInfo.onTableName + "-" + indexInfo.name;
    BTree<T> Tree = new BTree(fileName);
    
	bool tagInsert=Tree->insert(key, id);

	Tree->saveFile(fileName);
    delete Tree;

    return tagInsert;
}

template<typename T>
inline bool IndexManager<T>::delete_key(index indexInfo, T key, int id) {
    std::string fileName = indexInfo.onTableName + "-" + indexInfo.name;
    BTree<T> Tree = new BTree(fileName);

	bool tagDelete = Tree->Delete(key, id);

	Tree->saveFile(fileName);
    delete Tree;

    return false;
}
