#include "ArcGraph.hpp"

ArcGraph::ArcGraph(const IGraph &graph) {
    verticesCount = graph.VerticesCount();
    for (int from = 0; from < graph.VerticesCount(); from++) {
        for (int to : graph.GetNextVertices(from)) adjacencyPairs.push_back({from, to});
    }
}

void ArcGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < verticesCount);
    assert(0 <= to && to < verticesCount);
    adjacencyPairs.push_back({from, to});
}

int ArcGraph::VerticesCount() const { return verticesCount; }

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < verticesCount);
    std::vector<int> nextVertices;
    for (auto &pair : adjacencyPairs) {
        if (pair.first == vertex) nextVertices.push_back(pair.second);
    }
    return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < verticesCount);
    std::vector<int> prevVertices;
    for (auto &pair : adjacencyPairs) {
        if (pair.second == vertex) prevVertices.push_back(pair.first);
    }
    return prevVertices;
}
