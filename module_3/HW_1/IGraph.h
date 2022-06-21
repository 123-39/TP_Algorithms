//
// Created by trene on 25.05.2022.
//

#ifndef HW_1_IGRAPH_H
#define HW_1_IGRAPH_H

#include <vector>

struct IGraph {
    virtual ~IGraph() = default;

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

#endif //HW_1_IGRAPH_H
