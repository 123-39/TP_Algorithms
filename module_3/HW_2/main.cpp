/*
Дан невзвешенный неориентированный граф.
 В графе может быть несколько кратчайших путей между какими-то вершинами.
 Найдите количество различных кратчайших путей между заданными вершинами.

Требования: сложность O(V+E).

Формат ввода.
v: кол-во вершин (макс. 50000),
n: кол-во ребер (макс. 200000),
n пар реберных вершин,
пара вершин u, w для запроса.

Формат вывода.
Количество кратчайших путей от u к w.
 */

#include <iostream>
#include <vector>
#include <queue>

struct IGraph {
    virtual ~IGraph() = default;

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph
{
public:
    explicit ListGraph(std::size_t size) {
        graph.resize(size);
    }

    explicit ListGraph(const IGraph &input_graph)
    : graph(input_graph.VerticesCount()) {
        for (int i = 0; i < input_graph.VerticesCount(); i++)
            graph[i] = input_graph.GetNextVertices(i);
    }

    ~ListGraph() override = default;

    void AddEdge(int from, int to) override {
        graph[from].push_back(to);
    }

    int VerticesCount() const override {
        return static_cast<int>(graph.size());
    }

    std::vector<int> GetNextVertices(int vertex) const override {

        std::vector<int> result(graph[vertex].begin(),
                                graph[vertex].end());
        return result;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {

        std::vector<int> result;
        for (int i = 0; i < graph.size(); ++i) {
            for (int j = 0; j < graph[i].size(); ++j) {
                if (graph[i][j] == vertex)
                    result.push_back(i);
            }
        }

        return result;
    }

private:
    std::vector<std::vector<int>> graph;
};

void bfs(const IGraph &graph, int from, int to, void (*callback)(int))
{
    std::vector<int> lengh_way(graph.VerticesCount(), 0);
    std::vector<int> dist(graph.VerticesCount(), INT8_MAX);

    dist[from] = 0;
    lengh_way[from] = 1;

    std::queue<int> q;
    q.push(from);

    while (!q.empty()) {
        int cur_vert = q.front();
        q.pop();
        for (auto vert : graph.GetNextVertices(cur_vert)) {
            if (dist[vert] > dist[cur_vert] + 1) {
                dist[vert] = dist[cur_vert] + 1;
                q.push(vert);
            }
            if (dist[vert] == dist[cur_vert] + 1) {
                lengh_way[vert] += lengh_way[cur_vert];
            }
        }
    }
    callback(lengh_way[to]);
}

void run(ListGraph &graph, size_t number_edge) {

    int edge_from = 0, edge_to = 0;

    for (size_t i = 0; i < number_edge; ++i) {
        std::cin >> edge_from >> edge_to;
        graph.AddEdge(edge_from, edge_to);
        graph.AddEdge(edge_to, edge_from);
    }
}


int main () {
    // количество вершин и ребер
    size_t number_vert = 0, number_edge = 0;
    std::cin >> number_vert;
    std::cin >> number_edge;

    // инициализаци графа
    ListGraph graph(number_vert);

    // заполнение графа
    run(graph, number_edge);

    // поиск кратчайшего путя
    int way_start = 0, way_end = 0;
    std::cin >> way_start >> way_end;

    bfs(graph, way_start, way_end, [](int n_ways){ std::cout << n_ways << std::endl;});

    return 0;
}
