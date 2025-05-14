#include "MatrixGraph.hpp"

MatrixGraph::MatrixGraph(const IGraph &graph) {
    int size = graph.VerticesCount();
    adjacencyMatrix.resize(size, std::vector<int>(size, 0));
    for (int from = 0; from < graph.VerticesCount(); from++) {
        std::vector<int> nextVertices = graph.GetNextVertices(from);
        for (int to : nextVertices) {
            adjacencyMatrix[from][to]++;
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < adjacencyMatrix.size());
    assert(0 <= to && to < adjacencyMatrix.size());
    adjacencyMatrix[from][to]++;
}

int MatrixGraph::VerticesCount() const { return (int)adjacencyMatrix.size(); }

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjacencyMatrix.size());
    std::vector<int> nextVertices;
    for (int to = 0; to < adjacencyMatrix.size(); to++) {
        if (adjacencyMatrix[vertex][to]) nextVertices.push_back(to);
    }
    return nextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjacencyMatrix.size());
    std::vector<int> prevVertices;
    for (int from = 0; from < adjacencyMatrix.size(); from++) {
        if (adjacencyMatrix[from][vertex]) prevVertices.push_back(from);
    }
    return prevVertices;
}
