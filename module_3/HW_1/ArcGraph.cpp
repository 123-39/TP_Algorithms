//
// Created by trene on 26.05.2022.
//

#include "ArcGraph.h"


ArcGraph::ArcGraph(const IGraph &graph) {

    count_vert = graph.VerticesCount();
    for (auto i = 0; i < count_vert; ++i) {
        for (auto &to : graph.GetNextVertices(i)) {
            std::pair<int, int> pair(i, to);
            edge.push_back(pair);
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    edge.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const {
    return static_cast<int>(count_vert);
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {

    std::vector<int> next_vert;
    for (auto &edges : edge) {
        if (edges.first == vertex)
            next_vert.push_back(edges.second);
    }

    return next_vert;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {

    std::vector<int> prev_vert;
    for (auto &edges : edge) {
        if (edges.second == vertex)
            prev_vert.push_back(edges.first);
    }

    return prev_vert;
}
