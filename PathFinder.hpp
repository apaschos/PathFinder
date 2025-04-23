#pragma once

#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

#if 0
union Vec {
    struct {
        int x, y, z;
    };
    int data[3];

    bool operator==(const Vec& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vec& other) const {
        return !(*this == other);
    }

    Vec operator+(const Vec& other) const {
        return Vec{ x + other.x, y + other.y, z + other.z };
    }

    Vec& operator+=(const Vec& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }
};
#else
constexpr int LEN = 2;

union Vec {
    struct {
        int x, y;
    };
    int data[LEN];

    Vec(int x, int y) : x(x), y(y) {}

    int length() const { return LEN; }

    bool operator==(const Vec& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vec& other) const {
        return !(*this == other);
    }

    Vec operator+(const Vec& other) const {
        return Vec{ x + other.x, y + other.y };
    }

    Vec operator+(const std::pair<int, int>& other) const {
        return Vec{ x + other.first, y + other.second};
    }

    Vec& operator+=(const Vec& other) {
        x += other.x; y += other.y;;
        return *this;
    }
};

namespace std {
    template<>
    struct hash<Vec> {
        size_t operator()(const Vec& v) const noexcept {
            // Combine hashes of x and y using standard method
            size_t h1 = hash<int>{}(v.x);
            size_t h2 = hash<int>{}(v.y);
            // Bit manipulation for good distribution (from Boost's hash_combine)
            return h1 ^ (h2 << 1);
        }
    };
}

#endif

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
};

using Grid = std::vector<std::vector<int>>; // 2d map for now

class Pathfinder {
public:
    Pathfinder(const Grid& grid);
    std::vector<Node> findPath(const Vec& start, const Vec& end);

private:
    const Grid& grid_;
    int width_;
    int height_;

    bool isValid(const Vec& point) const;
    static double heuristic(const Vec& a, const Vec& b);
};
