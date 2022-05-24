#include <iostream>
#include "Graph.h"

void getNewGraphInput (int*, int*, int*, int*);

int main () {
    
    std::string fileName = "";
    std::cout << "Enter name of data file: ";
    std::cin >> fileName;
    
    Graph G;
    
    // Generate a new graph? Overwrite previous? Or read from file?
    char option;
    std::cout << "\nGenerate a new graph? Y/N: ";
    std::cin >> option;
    if (option == 'Y') {
        int n, e, minC, maxC;
        getNewGraphInput(&n, &e, &minC, &maxC);
        G.generateGraph(n, e, minC, maxC);
        std::cout << "\nOverwrite previous graph file? Y/N: ";
        std::cin >> option;
        if (option == 'Y')
            G.writeGraph(fileName);
    }
    else if (option == 'N')
        G.readGraph(fileName);

    // Ask user for source and sink.
    int s, t;
    std::cout << "\nNodes are labeled from 0 to " << G.n - 1 << "."
        << "\nSource: ";
    std::cin >> s;
    std::cout << "Sink: ";
    std::cin >> t;

    // Output max flow.
    std::cout << "\nMaximum flow from " << s << " to " << t << ": " 
        << G.dinics(s, t) << '\n';
    
    return 0;
}

void getNewGraphInput (int* n, int* e, int* minC, int* maxC) {
    std::cout << "Number of nodes: ";
    std::cin >> *n;
    std::cout << "Number of edges (between " << *n - 1 << " and "
        << *n * (*n - 1) / 2 << "): ";
    std::cin >> *e;
    std::cout << "Min capacity of an edge: ";
    std::cin >> *minC;
    std::cout << "Max capacity of an edge: ";
    std::cin >> *maxC;
}
