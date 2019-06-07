#ifndef DB_MINISQL_BPLUS_H
#define DB_MINISQL_BPLUS_H
#include <fstream>

namespace IM {
    template<typename T>
    struct BNode {
        T key;
        bool type;
        uint32_t size, father;
        std::vector<std::pair<uint32_t, T>> list;
        BNode* Nex;
    };

    template<typename T>
    class BPlus {
    private:
        BNode<T>* root;

    public:
        BPlus(std::string fileName);
        BPlus(CM::table& tableInfo);
    };

    template<typename T>
    BPlus<T>::BPlus(CM::table& tableInfo) {}

    template<typename T>
    BPlus<T>::BPlus(std::string fileName) {
        std::ifstream openFile(fileName);
        if (openFile.fail()) {
            // fail
        }
        bool isroot = 1;
        while (!openFile.eof()) {
            bool type;
            uint32_t size, father, child;
            T key;
            BNode<T> node;
            openFile >> type >> size >> father;
            node.type = type;
            node.size = size;
            node.father = father;
            for (int i = 0; i < size; i++) {
                openFile >> child >> key;
                node.list.push_back({child, key});
            }
            if (isroot) {
                isroot = 0;
                root = &node;
            }
        }
    }

}

#endif