#pragma once

struct Vec3 {
    int x, y, z;

    bool operator==(const Vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vec3& other) const {
        return !(*this == other);
    }

    Vec3 operator+(const Vec3& other) const {
        return Vec3{x + other.x, y + other.y, z + other.z};
    }

    Vec3 operator-(const Vec3& other) const {
        return Vec3{x - other.x, y - other.y, z - other.z};
    }
};

namespace std {
    template <>
    struct hash<Vec3> {
        std::size_t operator()(const Vec3& v) const {
            return ((std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1)) >> 1)
                ^ (std::hash<int>()(v.z) << 1);
        }
    };
}
