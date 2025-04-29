#pragma once

#include "Vec.hpp"

struct Node {
    Vec coords;
    double cost;
    double heuristic;
    Node* parent;

    bool operator>(const Node& other) const {
        return (cost + heuristic) > (other.cost + other.heuristic);
    }

    int x() const { return coords.x; }
    int y() const { return coords.y; }

    void reset() {
        coords.x = coords.y = 0;
        cost = 0.0;
        heuristic = 0.0;

        if (parent)
        {
            delete parent;
            parent = nullptr;
        }
    }
};
