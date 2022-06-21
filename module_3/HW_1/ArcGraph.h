//
// Created by trene on 26.05.2022.
//

#ifndef HW_1_ARCGRAPH_H
#define HW_1_ARCGRAPH_H


#include "IGraph.h"
#include <vector>
#include <algorithm>


class ArcGraph: public IGraph {
public:

    explicit ArcGraph(size_t size) : count_vert(size) {};

    explicit ArcGraph(const IGraph &graph);

    ~ArcGraph() override = default;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::pair<int, int>> edge;
    size_t count_vert;

};


#endif //HW_1_ARCGRAPH_H
