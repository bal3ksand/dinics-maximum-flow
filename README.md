# dinics-maximum-flow
Calculates maximum network flow in a graph in O(VE^2) time using [Dinic's algorithm](https://en.wikipedia.org/wiki/Dinic%27s_algorithm).

Based on [this](https://faculty.cc.gatech.edu/~rpeng/18434_S15/edmondsKarp2.pdf) and [this](https://faculty.cc.gatech.edu/~rpeng/18434_S15/blockingFlows.pdf)

Graph.cpp and Graph.h define an object "Graph" with the following functionality:
* **writeGraph(fileName)** - write the graph to a file.
* **readGraph(fileName)** - read a graph from a file.
* **generateGraph(n, e, minC, maxC)** - generate a simple connected weighted graph. no guarantee that there is a flow from source to sink. 
    * **n** - number of nodes
    - **e** - number of edges
    - **minC** - minimum capacity of an edge
    - **maxC** - maximum capacity of an edge
* **dinics(s, t)** - calculates maximum network flow from source **s** to sink **t**.
