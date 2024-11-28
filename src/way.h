#ifndef WAY_H
#define WAY_H

#include <iostream>
#include <vector>

using namespace std;


template<typename Type>
ostream & operator << (ostream& os, const vector<Type>& vec) {
    os << "[";
    for (size_t i = 0; i < vec.size(); i++) {
        os << vec[i];
        if (i < vec.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}


class Way {
public:
    std::vector<int> way;
    int len;


    Way() : way(vector<int>()), len(0) {}
    Way(int len) : way(vector<int>()), len(0) {}
    Way(std::vector<int> way, int len) : way(way), len(len) {}


    friend std::ostream& operator<<(std::ostream& os, const Way& object) {
        os << "Way (" << object.way << ", " << object.len << ")";
        return os;
    }

    int last() {
        return way[way.size() - 1];
    }

    int penultimate() {
        if (way.size() < 2)
            return -1;
        return way[way.size() - 2];
    }

    void add_way(int node, int len_way) {
        way.push_back(node);
        len += len_way;
    }

    int size() {
        return way.size();
    }
};


class compareByLength {
public:
    bool operator()(const Way& item, const Way& then) const {
        return item.len > then.len;
    }
};

#endif  // WAY_H
