//
// Created by trene on 25.05.2022.
//

#ifndef HW_1_LISTGRAPH_H
#define HW_1_LISTGRAPH_H

#include "IGraph.h"

class ListGraph : public IGraph
{
public:
    explicit ListGraph(std::size_t size);

    explicit ListGraph(const IGraph &input_graph);

    ~ListGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> graph;
};

#endif //HW_1_LISTGRAPH_H
