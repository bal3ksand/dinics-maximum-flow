#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <stack>

struct Edge {
    int v;    // edge (u, v)
    int flow;
    int capacity;
    long unsigned int rev_idx;    // index of edge (v, u) is adjList[u][rev_idx]
};

class Graph {
private:
    void addEdge(int u, int v, int capacity);
    bool assignLevels();
    int augmentPath(std::vector<int>& path);
    void findPaths(int currNode, int* roundFlow, std::vector<int>& path);
    int s, t;

public:
    int n;    // n vertices
    std::vector<Edge> *adjList;
    int *level;    // could be private
    
    Graph();
    ~Graph();

    void writeGraph(std::string fileName);
    void readGraph(std::string fileName);
    void generateGraph(int n, int e, int minC, int maxC);

    int dinics(int s, int t);
};

#endif
