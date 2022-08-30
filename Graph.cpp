#include <vector>
#include <queue>
#include <fstream>
#include <ctime>
#include <climits>
#include <algorithm>
#include "Graph.h"

Graph::Graph () {
}

Graph::~Graph () {
    delete[] adjList;
    delete[] level;
}

void Graph::addEdge (int u, int v, int capacity) {
// Adds a forward and a reverse edge.
// Edge: v, flow, capacity, rev_idx.
    Edge eF{v, 0, capacity, adjList[v].size()};
    adjList[u].push_back(eF);
    Edge eB{u, 0, 0, adjList[u].size()};
    adjList[v].push_back(eB);
}

void Graph::writeGraph (std::string fileName) {
// Writes forward edges to a file.
    std::ofstream graphFile;
    graphFile.open(fileName);
    graphFile << n << '\n';
    for (int u = 0; u < n; u++) {
        for (auto edge : adjList[u]) {
            // Reverse edges have capacity 0.
            if (edge.capacity > 0)
                graphFile << u << ' ' << edge.v << ' ' << edge.capacity << '\n';
        }
    }
    graphFile.close();
}

void Graph::readGraph (std::string fileName) {
// Reads from a file of forward edges.
// Adds both forward and reverse edges to the Graph.
    std::ifstream graphFile;
    graphFile.open(fileName);
    graphFile >> n;
    this->n = n;
    this->adjList = new std::vector<Edge>[n];
    this->level = new int[n];
    int u, v, capacity;
    while (graphFile >> u >> v >> capacity)
        addEdge(u, v, capacity);
    graphFile.close();
}

void Graph::generateGraph (int n, int e, int minC, int maxC) {
// Generates a random graph with 'n' nodes and 'e' edges.
// Forward edges have capacity [minC, maxC].
// Reverse edges have capacity 0.
// adapted from https://stackoverflow.com/a/14618505

    this->n = n;
    this->adjList = new std::vector<Edge>[n];
    this->level = new int[n];

    srand(time(NULL));

    // Adjacency Matrix to prevent duplicate edges.
    bool existsEdge[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            existsEdge[i][j] = false;
    }

    // S is the set of unconnected nodes.
    // T is the set of connected nodes.
    bool S[n], T[n];
    int Ssize = n;
    int nEdges = 0;
    for (int i = 0; i < n; i++) {
        S[i] = true;
        T[i] = false;
    }
    
    // Start with a random node from S and move it to T.
    int u, v;
    u = rand() % n;
    S[u] = false;
    T[u] = true;
    Ssize--;
    
    // Move nodes from S to T and add edges until S is empty.
    while (Ssize > 0) {
        // Pick a random node from all nodes to be potential 'v'.
        v = rand() % n;
        // If the random node v is in S, add a edge (u,v) and move v to T. 
        if (u != v && S[v]) { 
            addEdge(u, v, minC + rand() % (maxC - minC + 1));
            existsEdge[u][v] = true;
            existsEdge[v][u] = true;
            nEdges++;
            S[v] = false;
            T[v] = true;
            Ssize--;
        }
        // Regardless, the next edge will point from 'v'.
        u = v;
    }
    
    // Add random edges until nEdges == e.
    while (nEdges < e) {
        u = rand() % n;
        v = rand() % n;
        if (u != v && !existsEdge[u][v]) {
            addEdge(u, v, minC + rand() % (maxC - minC + 1));
            existsEdge[u][v] = true;
            existsEdge[v][u] = true;
            nEdges++;
        }
    }
}

bool Graph::assignLevels () {
// Updates the class array 'level' using BFS.
// level[node] is the distance from source to node on the shortest s-t path.
// The resulting BFS tree needs to be updated every round because paths are 
//   augmented, edges become saturated, and reverse flows become possible.
    for (int i = 0; i < n; i++)
        level[i] = -1;    // unexplored.
    std::queue<int> q;
    q.push(s);
    level[s] = 0;
    int currNode;
    while (!q.empty()) {
        currNode = q.front();
        q.pop();
        for (auto edge : adjList[currNode]) {
            if (level[edge.v] == -1 && edge.capacity - edge.flow > 0) {
                q.push(edge.v);
                level[edge.v] = level[currNode] + 1;
            }
        }
    }
    return level[t] == -1 ? false : true;
}

void Graph::findPaths(int currNode, int* roundFlow, std::vector<int>& path) {
// Uses DFS to find s-t paths along the level residual graph.
// When a path is found, it is augmented.
    if (currNode == t) {
        *roundFlow += augmentPath(path);
        return;
    }
    for (auto& edge : adjList[currNode]) {
        if (level[edge.v] == level[currNode] + 1 && edge.capacity - edge.flow > 0) {
            path.push_back(edge.v);
            findPaths(edge.v, roundFlow, path);
            path.pop_back();
        }
    }
}

int Graph::augmentPath (std::vector<int>& path) {
// Increases the flow on each path edge by the minimum residual capacity on the path.
    int min = INT_MAX;
    int residualCapacity;
    int u, v;
    Edge revEdge;
    
    // Find minimum residual capacity available on graph.
    for (int i = 0; i < path.size() - 1; i++) {
        u = path[i];
        v = path[i + 1];
        for (auto& e : adjList[u]) {
            if (e.v == v) {
                residualCapacity = e.capacity - e.flow;
                min = std::min(min, residualCapacity);
                break;
            }
        }
    }
    // Augment path by minimum residual capacity.
    for (int i = 0; i < path.size() - 1; i++) {
        u = path[i];
        v = path[i + 1];
        int rev_idx; // index of edge (v, u) is adjList[u][rev_idx].
        for (auto& e : adjList[u]) {
            if (e.v == v) {
                e.flow += min;
                revEdge = adjList[e.v][e.rev_idx];
                revEdge.flow -= min;
                break;
            }
        }
    }
    return min;
}

int Graph::dinics (int s, int t) {
// Implementation based on:
// https://faculty.cc.gatech.edu/~rpeng/18434_S15/edmondsKarp2.pdf
// https://faculty.cc.gatech.edu/~rpeng/18434_S15/blockingFlows.pdf
// https://www.cs.bgu.ac.il/~dinitz/Papers/Dinitz_alg.pdf
    this->s = s;
    this->t = t;

    std::vector<int> path;
    path.push_back(s);
    int roundFlow; // Flow found per round of level assignment.
    int totalFlow = 0;
    while (assignLevels()) {
        roundFlow = 0;
        findPaths(s, &roundFlow, path);
        totalFlow += roundFlow;
    }
    return totalFlow;
}
