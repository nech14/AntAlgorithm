#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <iomanip> 

using namespace std;

class MyGraph {

public:
    int countVertices;
    int start;
    int end;
    bool bidirectional = false;

    int** matrix;

    MyGraph() {
        countVertices = 1;
        start = 1;
        end = 1;

        matrix = new int* [countVertices];
        for (int i = 0; i < countVertices; ++i) {
            matrix[i] = new int[countVertices];
        }

        for (int i = 0; i < countVertices; ++i) {
            for (int j = 0; j < countVertices; ++j) {
                matrix[i][j] = i * countVertices + j;
            }
        }
    }

    MyGraph(const string& fileName) {
        ifstream file(fileName);

        int vertex1, vertex2, length;
        if (file.good()) {
            file >> countVertices;
            file >> start >> end;

            buildMatrix();

            while (file >> vertex1 >> vertex2 >> length) {
                matrix[vertex1][vertex2] = length;
            }
            file.close();
        }
        else {
            cout << "File is empty!" << endl;
        }
    }

    MyGraph(const string& fileName, bool bidirectional) : bidirectional(bidirectional) {
        ifstream file(fileName);

        int vertex1, vertex2, length;
        if (file.good()) {
            file >> countVertices >> start >> end;
            buildMatrix();

            while (file >> vertex1 >> vertex2 >> length) {
                matrix[vertex1][vertex2] = length;
                if (bidirectional) {
                    matrix[vertex2][vertex1] = length;
                }
            }
            file.close();
        }
        else {
            cout << "File is empty!" << endl;
        }
    }

    MyGraph(int countVertices, int start, int end) : countVertices(countVertices), start(start), end(end) {
        matrix = new int* [countVertices];
        for (int i = 0; i < countVertices; ++i) {
            matrix[i] = new int[countVertices];
        }

        for (int i = 0; i < countVertices; ++i) {
            for (int j = 0; j < countVertices; ++j) {
                matrix[i][j] = i * countVertices + j;
            }
        }
    }

    MyGraph(int countVertices, int start, int end, bool bidirectional)
        : countVertices(countVertices), start(start), end(end), bidirectional(bidirectional) {

        matrix = new int* [countVertices];
        for (int i = 0; i < countVertices; ++i) {
            matrix[i] = new int[countVertices];
        }

        for (int i = 0; i < countVertices; ++i) {
            for (int j = 0; j < countVertices; ++j) {
                matrix[i][j] = i * countVertices + j;
            }
        }
    }

    ~MyGraph() {
        for (int i = 0; i < countVertices; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    void buildMatrix() {
        matrix = new int* [countVertices];
        for (int i = 0; i < countVertices; ++i) {
            matrix[i] = new int[countVertices];
        }

        for (int i = 0; i < countVertices; ++i) {
            for (int j = 0; j < countVertices; ++j) {
                matrix[i][j] = 0;
            }
        }
    }

    Way deikstraAlgorithm() {
        priority_queue<Way, vector<Way>, compareByLength> puti;
        int way_start = start;

        for (int i = 0; i < countVertices; ++i) {
            if (i == way_start - 1) continue;
            int dlina = matrix[way_start][i];

            if (dlina <= 0) continue;

            vector<int> doroga = { way_start };
            doroga.push_back(i);
            puti.push(Way(doroga, dlina));
        }

        int target_peak = end;
        int end = puti.top().way[puti.top().way.size() - 1];

        while (end != target_peak) {
            Way bestPath = puti.top();
            puti.pop();
            way_start = bestPath.way[bestPath.way.size() - 1];

            for (int j = 0; j < countVertices; ++j) {
                if (find(bestPath.way.begin(), bestPath.way.end(), j + 1) != bestPath.way.end()) continue;

                int len_way = matrix[way_start][j];
                if (len_way <= 0) continue;

                len_way += bestPath.len;

                vector<int> doroga = bestPath.way;
                doroga.push_back(j);

                puti.push(Way(doroga, len_way));
            }
            end = puti.top().way[puti.top().way.size() - 1];
        }

        return puti.top();
    }

    friend ostream& operator<<(ostream& os, const MyGraph& obj) {
        os << "MyGraph: " << obj.countVertices << endl;

        int maxLen = 2;
        for (int i = 0; i < obj.countVertices; ++i) {
            for (int j = 0; j < obj.countVertices; ++j) {
                int len = to_string(obj.matrix[i][j]).length();
                maxLen = max(maxLen, len);
            }
        }

        for (int i = 0; i < obj.countVertices; ++i) {
            for (int j = 0; j < obj.countVertices; ++j) {
                os << setw(maxLen) << obj.matrix[i][j] << "  ";
            }
            os << endl;
        }
        return os;
    }

    vector<int> getAvailableEdges(int node) {
        vector<int> answer = vector<int>();


        for (int i = 0; i < countVertices; i++) {
        
            if (i == node)
                continue;

            if (matrix[node][i] > 0) {
                answer.push_back(i);
            }

        }


        return answer;
    }
};

#endif
