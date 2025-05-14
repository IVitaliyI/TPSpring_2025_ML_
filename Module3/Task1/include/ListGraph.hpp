#pragma once
#ifndef LISTGRAPH_HPP
#define LISTGRAPH_HPP

#include <cassert>
#include <vector>

#include "IGraph.hpp"

class ListGraph : public IGraph {
   private:
    std::vector<std::vector<int>> adjacencyLists;

   public:
    ListGraph(int size) : adjacencyLists(size) {}

    ListGraph(const IGraph &graph);

    ~ListGraph(){};

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
};

#endif  // LISTGRAPH_HPP