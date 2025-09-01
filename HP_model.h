#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <random>
#include "Vec3.h" // osobna struktura pozycji 3D

class HP_model {
public:
    HP_model();
    bool generuj_startowa_konformacje(int max_proby = 10000);
    bool przesun_koniec();
    void wypisz_konformacje() const;

private:
    std::string sekwencja_bialka;
    std::unordered_map<Vec3, char> mapa_pozycji;
    std::vector<Vec3> pozycje;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist_os;
    std::uniform_int_distribution<> dist_kierunek;

    int odleglosc(const Vec3& a, const Vec3& b) const;
};

