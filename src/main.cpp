#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

#include "BufferManager.h"
#include "CatalogManager.h"
#include "GlobalData.h"

void write() {
    CM::CatalogManager cm;
    CM::table& t = cm.getTable("book");

    Records data;
    data.push_back({std::to_string(1), "book1", "author1", std::to_string(12.4f)});
    data.push_back({std::to_string(2), "book2", "author2", std::to_string(1989.1f)});
    data.push_back({std::to_string(3), "sdgsg", "auther", std::to_string(1989.120f)});
    data.push_back({std::to_string(4), "sfhshsh", "sgshd", std::to_string(1989.0f)});
    data.push_back({std::to_string(5), "assdjfj", "CCCCCCCCCCCCCCCCCCC", std::to_string(1989.0f)});
    data.push_back({std::to_string(6), "fgjsfrkjsrk", "BBBBB", std::to_string(1989.0f)});
    data.push_back({std::to_string(7), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(8), "book1", "author1", std::to_string(12.4f)});
    data.push_back({std::to_string(9), "book2", "author2", std::to_string(1989.1f)});
    data.push_back({std::to_string(10), "sdgsg", "auther", std::to_string(1989.120f)});
    data.push_back({std::to_string(11), "sfhshsh", "sgshd", std::to_string(1989.0f)});
    data.push_back({std::to_string(12), "assdjfj", "CCCCCCCCCCCCCCCCCC", std::to_string(1989.0f)});
    data.push_back({std::to_string(13), "fgjsfrkjsrk", "BBBBB", std::to_string(1989.0f)});
    data.push_back({std::to_string(14), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(15), "book1", "author1", std::to_string(12.4f)});
    data.push_back({std::to_string(16), "book2", "author2", std::to_string(1989.1f)});
    data.push_back({std::to_string(17), "sdgsg", "auther", std::to_string(1989.120f)});
    data.push_back({std::to_string(18), "sfhshsh", "sgshd", std::to_string(1989.0f)});
    data.push_back({std::to_string(19), "assdjfj", "CCCCCCCCCCCCCC", std::to_string(1989.0f)});
    data.push_back({std::to_string(20), "fgjsfrkjsrk", "BBBBB", std::to_string(1989.0f)});
    data.push_back({std::to_string(21), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(22), "book1", "author1", std::to_string(12.4f)});
    data.push_back({std::to_string(23), "book2", "author2", std::to_string(1989.1f)});
    data.push_back({std::to_string(24), "sdgsg", "auther", std::to_string(1989.120f)});
    data.push_back({std::to_string(25), "sfhshsh", "sgshd", std::to_string(1989.0f)});
    data.push_back({std::to_string(26), "assdjfj", "CCCCCCCCCCCCCCCCCC", std::to_string(1989.0f)});
    data.push_back({std::to_string(27), "fgjsfrkjsrk", "BBBBB", std::to_string(1989.0f)});
    data.push_back({std::to_string(28), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(29), "fgjsfrkjsrk", "BBBBB", std::to_string(1989.0f)});
    data.push_back({std::to_string(30), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(31), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(32), "sgfjsrfjkrstykrsk", "AAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(33), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});

    char zero[256] = {0};

    std::ofstream os;
    os.open(t.name, std::ios::out | std::ios::binary);

    for (const auto& row : data) {
        for (unsigned int i = 0; i < t.NOF; ++i) {
            switch (t.fields[i].type) {
                case DataType::INT: {
                    int tmp = std::stoi(row[i]);
                    os.write(reinterpret_cast<const char*>(&tmp), sizeof(tmp));
                    break;
                }
                case DataType::CHAR_N: {
                    os.write(row[i].c_str(), row[i].size());
                    os.write(zero, t.fields[i].N - row[i].size());
                    break;
                }
                case DataType::FLOAT: {
                    float tmp = std::stof(row[i]);
                    os.write(reinterpret_cast<const char*>(&tmp), sizeof(tmp));
                    break;
                }
            }
        }
    }

    os.close();
}

int main() {
    write();

    BM::BufferManager bm;

    for (int i = 30; i < 33; ++i) {
        void* data = bm.read("book", i);
        int bookid = *static_cast<int*>(data);
        std::string title(static_cast<char*>(data) + 4, std::min((int)std::strlen(static_cast<char*>(data) + 4), 256));
        std::string author(static_cast<char*>(data) + 260,
                           std::min((int)std::strlen(static_cast<char*>(data) + 260), 256));
        float price = *(reinterpret_cast<float*>(static_cast<char*>(data) + 516));
        std::cout << bookid << " " << title << " " << author << " " << price << std::endl;
    }

    int idx;
    Records data;
    data.push_back({std::to_string(34), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(35), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(36), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(37), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(38), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(39), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(40), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(41), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(42), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(43), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(44), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(45), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(46), "sgfjsrfjkrstykrsk", "AAAAAA", std::to_string(1989.0f)});
    data.push_back({std::to_string(47), "cccsrsdgstykrsk", "AAAAAA", std::to_string(1239.0f)});
    data.push_back({std::to_string(48), "cccsrsdgstykrsk", "AAAAAA", std::to_string(1.0f)});
    data.push_back({std::to_string(49), "cccsrsdgstykrsk", "AAAAAA", std::to_string(2.0f)});
    data.push_back({std::to_string(50), "cccsrsdgstykrsk", "AAAAAA", std::to_string(3.0f)});
    data.push_back({std::to_string(51), "cccsrsdgstykrsk", "AAAAAA", std::to_string(1239.0f)});
    data.push_back({std::to_string(52), "cccsrsdgstykrsk", "AAAAAA", std::to_string(4.0f)});
    data.push_back({std::to_string(53), "cccsrsdgstykrsk", "AAAAAA", std::to_string(5.0f)});
    data.push_back({std::to_string(54), "cccsrsdgstykrsk", "AAAAAA", std::to_string(1239.0f)});
    data.push_back({std::to_string(55), "cccsrsdgstykrsk", "AAAAAA", std::to_string(6.0f)});

    for (const auto& r : data) {
        idx = bm.append("book", r);
    }
    bm.save(idx);

    for (int i = 33; i < 55; ++i) {
        void* data = bm.read("book", i);
        int bookid = *static_cast<int*>(data);
        std::string title(static_cast<char*>(data) + 4, std::min((int)std::strlen(static_cast<char*>(data) + 4), 256));
        std::string author(static_cast<char*>(data) + 260,
                           std::min((int)std::strlen(static_cast<char*>(data) + 260), 256));
        float price = *(reinterpret_cast<float*>(static_cast<char*>(data) + 516));
        std::cout << bookid << " " << title << " " << author << " " << price << std::endl;
    }

    return 0;
}