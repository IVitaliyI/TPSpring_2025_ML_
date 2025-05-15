
#include <cassert>
#include <climits>
#include <functional>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

struct IGraph {
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

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

ListGraph::ListGraph(const IGraph &graph) {
    adjacencyLists.resize(graph.VerticesCount());
    for (int i = 0; i < graph.VerticesCount(); i++) {
        adjacencyLists[i] = graph.GetNextVertices(i);
    }
}

void ListGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < adjacencyLists.size());
    assert(0 <= to && to < adjacencyLists.size());
    adjacencyLists[from].push_back(to);
}

int ListGraph::VerticesCount() const { return (int)adjacencyLists.size(); }

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < adjacencyLists.size());
    return adjacencyLists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < adjacencyLists.size());
    std::vector<int> prevVertices;
    for (int from = 0; from < adjacencyLists.size(); from++) {
        for (int to : adjacencyLists[from]) {
            if (to == vertex) prevVertices.push_back(from);
        }
    }
    return prevVertices;
}

void BFS(const IGraph &graph, int StartVertex, std::vector<int> &distances,
         std::vector<int> &pathsCounts) {
    std::queue<int> qu;
    distances[StartVertex] = 0;
    pathsCounts[StartVertex] = 1;
    qu.push(StartVertex);

    while (!qu.empty()) {
        int current = qu.front();
        qu.pop();

        for (int next : graph.GetNextVertices(current)) {
            if (distances[next] == -1) {
                distances[next] = distances[current] + 1;
                pathsCounts[next] = pathsCounts[current];
                qu.push(next);
            } else if (distances[next] == distances[current] + 1) {
                pathsCounts[next] += pathsCounts[current];
            }
        }
    }
}

void task_main(std::istream &input, std::ostream &output) {
    int v, n;
    int from, to;
    int startVertex, currentVertex;
    input >> v >> n;
    ListGraph g(v);
    for (int i = 0; i < n; i++) {
        input >> from >> to;
        g.AddEdge(from, to);
        g.AddEdge(to, from);
    }
    input >> startVertex >> currentVertex;
    std::vector<int> distance(g.VerticesCount(), -1);
    std::vector<int> pathsCount(g.VerticesCount(), 0);
    BFS(g, startVertex, distance, pathsCount);
    output << pathsCount[currentVertex] << std::endl;
}

int main() { task_main(std::cin, std::cout); }
