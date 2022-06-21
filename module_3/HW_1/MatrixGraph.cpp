//
// Created by trene on 25.05.2022.
//

#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(std::size_t size) {
    graph.resize(size, std::vector<bool>(size, false));
}

MatrixGraph::MatrixGraph(const IGraph &input_graph) {
    graph.resize(input_graph.VerticesCount(),
                 std::vector<bool>(input_graph.VerticesCount(), false));
    std::vector<int> verts;
    for (int i = 0; i < input_graph.VerticesCount(); ++i) {
        verts = input_graph.GetNextVertices(i);
        for (auto &vert : verts)
            graph[i][vert] = true;
    }
}
void MatrixGraph::AddEdge(int from, int to) {
    graph[from][to] = true;
}

int MatrixGraph::VerticesCount() const {
    return static_cast<int>(graph.size());
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {

    std::vector<int> next_vert = {};
    for (int i = 0; i < graph.size(); ++i) {
        if (graph[vertex][i])
            next_vert.push_back(i);
    }

    return next_vert;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const{

    std::vector<int> prevVertices = {};
    for (int j = 0; j < graph.size(); j++) {
        if (graph[j][vertex])
            prevVertices.push_back(j);
    }

    return prevVertices;
}
