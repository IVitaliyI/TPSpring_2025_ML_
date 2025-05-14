#include "SetGraph.hpp"

SetGraph::SetGraph(const IGraph &graph) {
    int size = graph.VerticesCount();
    adjacencyMap.resize(size);
    for (int from = 0; from < size; from++) {
        std::vector<int> nextVertices = graph.GetNextVertices(from);
        for (int to : nextVertices) {
            adjacencyMap[from][to]++;
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < adjacencyMap.size());
    assert(0 <= to && to < adjacencyMap.size());
    adjacencyMap[from][to]++;
}

int SetGraph::VerticesCount() const { return (int)adjacencyMap.size(); }

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjacencyMap.size());
    std::vector<int> nextVertices;
    for (auto &pair : adjacencyMap[vertex]) {
        nextVertices.push_back(pair.first);
    }
    return nextVertices;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjacencyMap.size());
    std::vector<int> prevVertices;
    for (int from = 0; from < adjacencyMap.size(); from++) {
        if (adjacencyMap[from].count(vertex)) {
            prevVertices.push_back(from);
        }
    }
    return prevVertices;
}
