//
// Created by trene on 25.05.2022.
//

#ifndef HW_1_MATRIXGRAPH_H
#define HW_1_MATRIXGRAPH_H

#include "IGraph.h"
#include <vector>

class MatrixGraph : public IGraph
{
public:
    explicit MatrixGraph(std::size_t size);

    explicit MatrixGraph(const IGraph &input_graph);

    ~MatrixGraph() override = default;;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<bool>> graph;
};


#endif //HW_1_MATRIXGRAPH_H
