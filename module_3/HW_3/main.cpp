/*
Требуется отыскать самый выгодный маршрут между городами.

Требования: время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.

Формат входных данных.
 Первая строка содержит число N – количество городов.
 Вторая строка содержит число M - количество дорог.
 Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
 Последняя строка содержит маршрут (откуда и куда нужно доехать).

Формат выходных данных.
 Вывести длину самого выгодного маршрута.
 */

#include <iostream>
#include <vector>
#include <set>

struct IGraph {
    virtual ~IGraph() = default;

    virtual void AddEdge(int from, int to, int cost) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const = 0;
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

    void AddEdge(int from, int to, int cost) override {
        graph[from].emplace_back(to, cost);
    }

    int VerticesCount() const override {
        return static_cast<int>(graph.size());
    }

    std::vector<std::pair<int, int>> GetNextVertices(int vertex) const override {

        std::vector<std::pair<int, int>> result(graph[vertex].begin(),
                                graph[vertex].end());
        return result;
    }

private:
    std::vector<std::vector<std::pair<int, int>>> graph;
};

void shortest_way(const IGraph &graph, int from, int to, void (*callback)(int)) {

    std::vector<int> dist(graph.VerticesCount(), INT32_MAX);
    std::set<std::pair<int, int>> prior_q;

    dist[from] = 0;
    prior_q.emplace(0, from);

    while (!prior_q.empty()) {
        // берем вершину, с наименьшей стоимостью
        auto cur_vert = *prior_q.begin();
        // убираем из очереди рассматриваемую вершину
        prior_q.erase(prior_q.begin());
        // пробегаем от нее по всем детям
        for (auto vert : graph.GetNextVertices(cur_vert.second)) {
            // релаксация
            if (dist[vert.first] > dist[cur_vert.second] + vert.second) {
                prior_q.erase(std::pair<int, int>(dist[vert.first], vert.first));
                dist[vert.first] = dist[cur_vert.second] + vert.second;
                // добавляем новый путь в очередь
                prior_q.insert(std::pair<int, int>(dist[vert.first], vert.first));
            }
        }

    }
    callback(dist[to]);
}


void run(ListGraph &graph, size_t number_edge) {

    int edge_from = 0, edge_to = 0, cost = 0;

    for (size_t i = 0; i < number_edge; ++i) {
        std::cin >> edge_from >> edge_to >> cost;
        graph.AddEdge(edge_from, edge_to, cost);
        graph.AddEdge(edge_to, edge_from, cost);
    }
}


int main () {
    // количество вершин (городов) и ребер (дорог)
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

    shortest_way(graph, way_start, way_end,
                 [](int distance) { std::cout << distance << std::endl; });

    return 0;
}

