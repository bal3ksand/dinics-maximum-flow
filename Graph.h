#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

struct Edge {
    int v;    // edge (u, v).
    int flow;
    int capacity;
    long unsigned int rev_idx;    // idx of edge (v, u) is adjList[u][rev_idx].
                                  // adjList[i].size() is typed as long
                                  //    unsigned int by the compilers, so
                                  //    rev_idx type must match to assign value.
};

class Graph {
private:
    int s, t;
    std::vector<Edge> *adjList;
    int *level;

    void addEdge(int u, int v, int capacity);
    bool assignLevels();
    void findPaths(int currNode, int* roundFlow, std::vector<int>& path);
    int augmentPath(std::vector<int>& path);

public:
    int n;    // 'n' vertices.
    
    Graph();
    ~Graph();

    void writeGraph(std::string fileName);
    void readGraph(std::string fileName);
    void generateGraph(int n, int e, int minC, int maxC);
    int dinics(int s, int t);
};

#endif
