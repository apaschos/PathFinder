#pragma once

#include <unordered_map>


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

    Vec() : x(-1), y(-1) {}
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
        return Vec{ x + other.first, y + other.second };
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
