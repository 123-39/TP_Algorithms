//
// Created by trene on 25.05.2022.
//

#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"
#include "IGraph.h"
#include <queue>
#include <iostream>

void bfs(const IGraph &graph, void (*callback)(int))
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> q;
    // перебираем все вершины графа
    for (int i = 0; i < graph.VerticesCount(); i++) {
        // если еще не посетили вершину,
        if (!visited[i]) {
            // то посещаем
            visited[i] = true;
            callback(i);
            // двигаемся дальше
            q.push(i);
            while (!q.empty()) {
                // выталкиваем вершину
                int cur_v = q.front();
                q.pop();
                // посещаем детей
                for (int child: graph.GetNextVertices(cur_v)) {
                    if (!visited[child]) {
                        visited[child] = true;
                        callback(child);
                        q.push(child);
                    }
                }
            }
        }
    }
}

void dfs_aux(const IGraph& graph, int vertex, std::vector<bool>& visited, void (*callback)(int v)) {
    visited[vertex] = true;
    callback(vertex);

    for (auto child : graph.GetNextVertices(vertex)) {
        if (!visited[child])
            dfs_aux(graph, child, visited, callback);
    }
}

void dfs(const IGraph& graph, void (*callback)(int v)) {
    std::vector<bool> visited;
    visited.resize(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i])
            dfs_aux(graph, i, visited, callback);
    }
}

template <class T_prev, class T_now>
T_now run(T_prev prev_graph) {
    T_now graph(7);
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 5);
    graph.AddEdge(4, 2);
    graph.AddEdge(1, 3);
    graph.AddEdge(1, 5);
    graph.AddEdge(1, 6);
    graph.AddEdge(3, 2);
    graph.AddEdge(3, 4);
    graph.AddEdge(2, 6);
    graph.AddEdge(5, 4);
    graph.AddEdge(5, 6);
    graph.AddEdge(6, 4);

    std::cout << "--BFS--" << std::endl;
    bfs(graph, [](int v) { std::cout << v << " "; });
    std::cout << "\n--DFS--" << std::endl;
    dfs(graph, [](int v) { std::cout << v << " "; });
    std::cout << "\n" << std::endl;

    T_now graph_2(prev_graph);
    std::cout << "--BFS--" << std::endl;
    bfs(graph_2, [](int v) { std::cout << v << " "; });
    std::cout << "\n--DFS--" << std::endl;
    dfs(graph_2, [](int v) { std::cout << v << " "; });
    std::cout << "\n" << std::endl;

    return graph;
}

int main() {

    std::cout << "1.)-------ListGraph-------\n";
    ListGraph graph(7);
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 5);
    graph.AddEdge(4, 2);
    graph.AddEdge(1, 3);
    graph.AddEdge(1, 5);
    graph.AddEdge(1, 6);
    graph.AddEdge(3, 2);
    graph.AddEdge(3, 4);
    graph.AddEdge(2, 6);
    graph.AddEdge(5, 4);
    graph.AddEdge(5, 6);
    graph.AddEdge(6, 4);
    ListGraph list_graph = run<ListGraph, ListGraph>(graph);

    std::cout << "2.)-------MatrixGraph-------\n";
    MatrixGraph matrix_graph = run<ListGraph, MatrixGraph>(list_graph);

    std::cout << "3.)-------SetGraph-------\n";
    SetGraph set_graph = run<MatrixGraph, SetGraph>(matrix_graph);

    std::cout << "4.)-------ArcGraph-------\n";
    ArcGraph arc_graph = run<SetGraph, ArcGraph>(set_graph);

    return 0;
}
