#pragma once
#ifndef ARCGRAPH_HPP
#define ARCGRAPH_HPP

#include <cassert>
#include <unordered_map>

#include "IGraph.hpp"

class ArcGraph : public IGraph {
   private:
    std::vector<std::pair<int, int>> adjacencyPairs;
    int verticesCount;

   public:
    ArcGraph(int size) : adjacencyPairs(size), verticesCount(0) {}

    ArcGraph(const IGraph &graph);

    ~ArcGraph() override = default;

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
};

#endif  // ARCGRAPH_HPP