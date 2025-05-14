#pragma once
#ifndef MATRIXGRAPH_HPP
#define MATRIXGRAPH_HPP

#include <cassert>
#include <vector>

#include "IGraph.hpp"

class MatrixGraph : public IGraph {
   private:
    std::vector<std::vector<int>> adjacencyMatrix;

   public:
    MatrixGraph(int size) : adjacencyMatrix(size, std::vector<int>(size, 0)) {}

    MatrixGraph(const IGraph &graph);

    ~MatrixGraph() override = default;

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
};

#endif  // MATRIXGRAPH_HPP