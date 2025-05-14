#include <functional>
#include <iostream>
#include <queue>

#include "ArcGraph.hpp"
#include "ListGraph.hpp"
#include "MatrixGraph.hpp"
#include "SetGraph.hpp"

void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited,
         const std::function<void(int)> &func) {
    std::queue<int> qu;
    qu.push(vertex);
    visited[vertex] = true;

    while (!qu.empty()) {
        int currentVertex = qu.front();
        qu.pop();

        func(currentVertex);

        for (int nextVertex : graph.GetNextVertices(currentVertex)) {
            if (!visited[nextVertex]) {
                visited[nextVertex] = true;
                qu.push(nextVertex);
            }
        }
    }
}

void mainBFS(const IGraph &graph, const std::function<void(int)> &func) {
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            BFS(graph, i, visited, func);
        }
    }
}

void DFS(const IGraph &graph, int vertex, std::vector<bool> &visited,
         const std::function<void(int)> &func) {
    visited[vertex] = true;
    func(vertex);

    for (int nextVertex : graph.GetNextVertices(vertex)) {
        if (!visited[nextVertex]) {
            DFS(graph, nextVertex, visited, func);
        }
    }
}

void mainDFS(const IGraph &graph, const std::function<void(int)> &func) {
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            DFS(graph, i, visited, func);
        }
    }
}

void topologicalSortInternal(const IGraph &graph, int vertex, std::vector<bool> &visited,
                             std::deque<int> &sorted) {
    visited[vertex] = true;

    for (int nextVertex : graph.GetNextVertices(vertex)) {
        if (!visited[nextVertex]) {
            topologicalSortInternal(graph, nextVertex, visited, sorted);
        }
    }

    sorted.push_front(vertex);
}

std::deque<int> topologicalSort(const IGraph &graph) {
    std::deque<int> sorted;
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            topologicalSortInternal(graph, i, visited, sorted);
        }
    }

    return sorted;
}

void test(IGraph &graph) {
    std::cout << std::endl;

    mainBFS(graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    mainDFS(graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    for (int vertex : topologicalSort(graph)) {
        std::cout << vertex << " ";
    }

    std::cout << std::endl;
    std::cout << std::endl;
}

int main() {
    ListGraph lgraph(7);

    lgraph.AddEdge(0, 1);
    lgraph.AddEdge(0, 5);
    lgraph.AddEdge(1, 2);
    lgraph.AddEdge(1, 3);
    lgraph.AddEdge(1, 5);
    lgraph.AddEdge(1, 6);
    lgraph.AddEdge(3, 2);
    lgraph.AddEdge(3, 4);
    lgraph.AddEdge(3, 6);
    lgraph.AddEdge(5, 4);
    lgraph.AddEdge(5, 6);
    lgraph.AddEdge(6, 4);

    std::cout << "--------------List graph---------------" << std::endl;
    test(lgraph);
    std::cout << "--------------Matrix graph---------------" << std::endl;
    MatrixGraph mgraph(lgraph);
    test(mgraph);
    std::cout << "--------------Set graph---------------" << std::endl;
    SetGraph sgraph(mgraph);
    test(sgraph);
    std::cout << "--------------ARC graph---------------" << std::endl;
    ArcGraph agraph(lgraph);
    test(agraph);
}