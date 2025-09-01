#pragma once

// Struktura 3D reprezentująca pozycję aminokwasu na siatce
struct Vec3 {
    int x, y, z;

    // Porównanie dwóch pozycji (potrzebne do unordered_map)
    bool operator==(const Vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    // Dodawanie wektorów
    Vec3 operator+(const Vec3& other) const {
        return Vec3{x + other.x, y + other.y, z + other.z};
    }
};

// Funkcja haszująca dla Vec3 — pozwala używać go jako klucza w unordered_map
namespace std {
    template <>
    struct hash<Vec3> {
        std::size_t operator()(const Vec3& v) const {
            return ((std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1)) >> 1) ^ (std::hash<int>()(v.z) << 1);
        }
    };
}

