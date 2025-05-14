#pragma once
#ifndef SETGRAPH_HPP
#define SETGRAPH_HPP

#include <cassert>
#include <unordered_map>

#include "IGraph.hpp"

class SetGraph : public IGraph {
   private:
    std::vector<std::unordered_map<int, int>> adjacencyMap;

   public:
    SetGraph(int size) : adjacencyMap(size) {}

    SetGraph(const IGraph &graph);

    ~SetGraph() override = default;

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
};

#endif  // SETGRAPH_HPP