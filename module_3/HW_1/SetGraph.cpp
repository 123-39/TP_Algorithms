//
// Created by trene on 26.05.2022.
//

#include "SetGraph.h"

SetGraph::SetGraph(const IGraph &input_graph) : graph(input_graph.VerticesCount()) {

    for (int i = 0; i < input_graph.VerticesCount(); ++i) {
        for (auto &next_vert : input_graph.GetNextVertices(i))
            graph[i].insert(next_vert);
    }
}

void SetGraph::AddEdge(int from, int to)  {
    graph[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return static_cast<int>(graph.size());;
}


std::vector<int> SetGraph::GetNextVertices(int vertex) const {

    std::vector<int> next_verts(graph.size());
    for (auto value: graph[vertex])
        next_verts.push_back(value);

    return next_verts;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {

    std::vector<int> prev_verts;
    for (int i = 0; i < graph.size(); ++i) {
        if (graph[i].count(vertex))
            prev_verts.push_back(i);
    }

    return prev_verts;
}
