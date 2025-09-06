#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <random>
#include "Vec3.h"

/**
 * Klasa HP_model: modeluje zwijanie białka w modelu HP na 3D siatce.
 * Obsługuje trzy ruchy: przesunięcie końca, obrót narożnika, crankshaft.
 * Implementuje algorytm Metropolisa z symulowanym wyżarzaniem.
 */

class HP_model {
public:
    HP_model();

    /**
     * Inicjalizuje konformację białka w linii lub losowo (self-avoiding walk).
     * @param losowa true - losowa, false - linia prosta
     * @param max_proby maksymalna liczba prób wygenerowania konformacji
     * @return true jeśli udało się wygenerować konformację
     */
    bool generuj_startowa_konformacje(bool losowa = true, int max_proby = 10000);

    /**
     * Symulacja zwijania metodą Metropolisa z wyżarzaniem.
     * @param T0 początkowa temperatura
     * @param T_inf minimalna temperatura
     * @param alpha współczynnik chłodzenia
     * @param steps liczba kroków symulacji
     */
    void algorytm_metropolisa(double T0, double T_inf, double alpha, int steps);

    /**
     * Wypisuje statystyki ruchów po zakończeniu symulacji.
     */
    void wypisz_statystyki() const;

    /**
     * Zwraca aktualne pozycje aminokwasów.
     */
    const std::vector<Vec3>& get_pozycje() const { return pozycje; }
    double oblicz_energie() const;

    /**
     * Gettery do statystyk akceptowanych ruchów.
     */
    int get_zaakceptowane_koniec() const { return zaakceptowane_koniec; }
    int get_zaakceptowane_naroznik() const { return zaakceptowane_naroznik; }
    int get_zaakceptowane_crankshaft() const { return zaakceptowane_crankshaft; }

private:
    std::string sekwencja_bialka;
    std::unordered_map<Vec3, char> mapa_pozycji;
    std::vector<Vec3> pozycje;

    std::mt19937 gen;
    std::uniform_int_distribution<> dist_os;
    std::uniform_int_distribution<> dist_kierunek;
    std::uniform_int_distribution<> dist_ruch;

    // Statystyki ruchów w symulacji
    int proponowane_koniec, zaakceptowane_koniec;
    int proponowane_naroznik, zaakceptowane_naroznik;
    int proponowane_crankshaft, zaakceptowane_crankshaft;
    int nieudane_koniec, nieudane_naroznik, nieudane_crankshaft;

    // Pomocnicze funkcje/model ruchów
    int odleglosc(const Vec3& a, const Vec3& b) const;
    bool pole_wolne(const Vec3& pos) const;
    bool sa_sasiadami(const Vec3& a, const Vec3& b) const; 

    std::vector<Vec3> ruch_przesun_koniec();
    std::vector<Vec3> ruch_obrot_naroznika();
    std::vector<Vec3> ruch_crankshaft();
};
