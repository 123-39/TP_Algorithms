//
// Created by trene on 26.05.2022.
//

#ifndef HW_1_SETGRAPH_H
#define HW_1_SETGRAPH_H


#include "IGraph.h"
#include <vector>
#include <set>

class SetGraph : public IGraph {
public:

    explicit SetGraph(std::size_t size) : graph(size) {}

    explicit SetGraph(const IGraph &input_graph);

    ~SetGraph() override = default;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::set<int>> graph;

};


#endif //HW_1_SETGRAPH_H
