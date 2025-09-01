#include "HP_model.h"
#include <iostream>
#include <chrono>

// Opisywane Białko
const std::string bialko = "PHPHHHHHPHPHPHHPPPPPHPPPPHHHPPPPPHPPPHHPHPHHHHPPPPHHHHPPHPHPHHHHHHHHHPPHHPP";

// Konstruktor klasy — inicjalizuje RNG oraz sekwencję białka
HP_model::HP_model()
    : sekwencja_bialka(domyslne_bialko),
      dist_os(0, 2),        // losowanie osi: x=0, y=1, z=2
      dist_kierunek(0, 1)   // losowanie kierunku: 0 → -1, 1 → +1
{
    // Ustawienie ziarna generatora losowego na podstawie czasu
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    gen.seed(seed);
}

// Funkcja obliczająca odległość Manhattan (1 jednostka = sąsiedztwo)
int HP_model::odleglosc(const Vec3& a, const Vec3& b) const {
    return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
}

// Generowanie startowej konformacji poprzez losowe ruchy przestrzenne bez kolizji
bool HP_model::generuj_startowa_konformacje(int max_proby) {
    mapa_pozycji.clear();
    pozycje.clear();

    Vec3 pos{0, 0, 0};  // Startujemy od pozycji (0,0,0)
    pozycje.push_back(pos);
    mapa_pozycji[pos] = sekwencja_bialka[0];

    // Iterujemy przez całą sekwencję białka
    for (size_t i = 1; i < sekwencja_bialka.size(); ++i) {
        bool znaleziono = false;

        for (int proba = 0; proba < max_proby; ++proba) {
            Vec3 nowa_poz = pos;

            // Losujemy oś i kierunek
            int os = dist_os(gen);
            int kier = dist_kierunek(gen) == 0 ? -1 : 1;

            if (os == 0) nowa_poz.x += kier;
            else if (os == 1) nowa_poz.y += kier;
            else nowa_poz.z += kier;

            // Sprawdzenie kolizji (czy miejsce jest już zajęte)
            if (mapa_pozycji.find(nowa_poz) == mapa_pozycji.end()) {
                pos = nowa_poz;
                pozycje.push_back(pos);
                mapa_pozycji[pos] = sekwencja_bialka[i];
                znaleziono = true;
                break;
            }
        }

        if (!znaleziono) {
            std::cerr << "Błąd: Nie udało się wygenerować konformacji (brak miejsca)\n";
            return false;
        }
    }

    return true;
}

// Wypisuje aktualną konformację w formacie: x y z H/P
void HP_model::wypisz_konformacje() const {
    for (size_t i = 0; i < pozycje.size(); ++i) {
        const Vec3& p = pozycje[i];
        std::cout << p.x << " " << p.y << " " << p.z << "   " << sekwencja_bialka[i] << "\n";
    }
}

// Ruch końcem łańcucha — losowo wybieramy początek lub koniec i próbujemy go przesunąć
bool HP_model::przesun_koniec() {
    if (pozycje.size() < 2) return false; // Nie można przesunąć końca jeśli nie ma więcej niż 1 aminokwas

    // Losowy wybór końca: 0 = początek, 1 = koniec
    bool czy_koniec = std::uniform_int_distribution<>(0, 1)(gen);
    size_t idx = czy_koniec ? pozycje.size() - 1 : 0;
    size_t sasiad_idx = czy_koniec ? pozycje.size() - 2 : 1;

    Vec3 sasiad = pozycje[sasiad_idx];
    Vec3 nowa_poz = sasiad;

    // Losujemy nową pozycję oddaloną o 1
    int os = dist_os(gen);
    int kier = dist_kierunek(gen) == 0 ? -1 : 1;

    if (os == 0) nowa_poz.x += kier;
    else if (os == 1) nowa_poz.y += kier;
    else nowa_poz.z += kier;

    // Sprawdzamy, czy pozycja jest wolna i poprawna
    if (mapa_pozycji.find(nowa_poz) != mapa_pozycji.end()) return false;
    if (odleglosc(nowa_poz, sasiad) != 1) return false;

    // Wykonujemy przesunięcie
    mapa_pozycji.erase(pozycje[idx]);
    pozycje[idx] = nowa_poz;
    mapa_pozycji[nowa_poz] = sekwencja_bialka[idx];

    return true;
}
	
	


