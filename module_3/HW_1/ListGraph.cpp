//
// Created by trene on 25.05.2022.
//

#include "ListGraph.h"

ListGraph::ListGraph(std::size_t verticesCount) {
    graph.resize(verticesCount);
}

ListGraph::ListGraph(const IGraph &input_graph)
        : graph(input_graph.VerticesCount()) {
    for (int i = 0; i < input_graph.VerticesCount(); i++)
        graph[i] = input_graph.GetNextVertices(i);
}

ListGraph::~ListGraph() = default;

void ListGraph::AddEdge(int from, int to) {
    graph[from].push_back(to);
}

int ListGraph::VerticesCount() const {
    return static_cast<int>(graph.size());
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {

    std::vector<int> result(graph[vertex].begin(),
                            graph[vertex].end());
    return result;
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const
{
    std::vector<int> result;
    for (int i = 0; i < graph.size(); ++i) {
        for (int j = 0; j < graph[i].size(); ++j) {
            if (graph[i][j] == vertex)
                result.push_back(i);
        }
    }

    return result;
}
