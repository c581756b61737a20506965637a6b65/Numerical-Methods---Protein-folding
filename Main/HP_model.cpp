#include "HP_model.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <random>
#include <algorithm>

/**
 * Konstruktor: ustawia sekwencję białka, inicjalizuje generator liczb losowych i zeruje statystyki.
 */
HP_model::HP_model()
    : gen(std::random_device{}()), dist_os(0,2), dist_kierunek(0,5), dist_ruch(0,2),
      nieudane_koniec(0), nieudane_naroznik(0), nieudane_crankshaft(0)
{
    // Sekwencja ubikwityny w kodzie HP (przykład)
    sekwencja_bialka = "PHPHHHHHPHPHPHHPPPPPHPPPPHHHPPPPPHPPPHHPHPHHHHPPPPHHHHPPHPHPHHHHHHHHHPPHHPP";
    proponowane_koniec = zaakceptowane_koniec = 0;
    proponowane_naroznik = zaakceptowane_naroznik = 0;
    proponowane_crankshaft = zaakceptowane_crankshaft = 0;
}

/**
 * Inicjalizacja: generuje linię prostą lub losowy walk bez kolizji.
 */
bool HP_model::generuj_startowa_konformacje(bool losowa, int max_proby) {
    pozycje.clear();
    mapa_pozycji.clear();
    std::vector<Vec3> mozliwe_ruchy = {
        Vec3{1,0,0}, Vec3{-1,0,0}, Vec3{0,1,0}, Vec3{0,-1,0}, Vec3{0,0,1}, Vec3{0,0,-1}
    };

    if (!losowa) {
        // Linia prosta wzdłuż osi Z
        for (size_t i = 0; i < sekwencja_bialka.length(); ++i) {
            Vec3 pos = Vec3{0, 0, static_cast<int>(-i)};
            pozycje.push_back(pos);
            mapa_pozycji[pos] = sekwencja_bialka[i];
        }
        return true;
    }

    // Losowy walk z unikaniem kolizji
    pozycje.push_back(Vec3{0,0,0});
    mapa_pozycji[pozycje.back()] = sekwencja_bialka[0];
    for (size_t i = 1; i < sekwencja_bialka.length(); ++i) {
        std::vector<Vec3> kandydaci;
        Vec3 ostatni = pozycje.back();
        for (const auto& ruch : mozliwe_ruchy) {
            Vec3 nowy = ostatni + ruch;
            if (mapa_pozycji.find(nowy) == mapa_pozycji.end())
                kandydaci.push_back(nowy);
        }
        if (kandydaci.empty()) {
            std::cerr << "Nie udało się wygenerować losowej konformacji, restartuję...\n";
            return false;
        }
        Vec3 wybrany = kandydaci[dist_kierunek(gen) % kandydaci.size()];
        pozycje.push_back(wybrany);
        mapa_pozycji[wybrany] = sekwencja_bialka[i];
    }
    return true;
}

/**
 * Odległość Manhattan pomiędzy dwoma punktami.
 */
int HP_model::odleglosc(const Vec3& a, const Vec3& b) const {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
}

/**
 * Sprawdza, czy dwa punkty są bezpośrednimi sąsiadami na siatce 3D.
 */
bool HP_model::sa_sasiadami(const Vec3& a, const Vec3& b) const {
    return odleglosc(a, b) == 1;
}

/**
 * Energia HP: -1 za każdy niesąsiedni kontakt H-H.
 */
double HP_model::oblicz_energie() const {
    double energia = 0.0;
    for (size_t i = 0; i < pozycje.size(); ++i) {
        if (sekwencja_bialka[i] != 'H') continue;
        for (size_t j = i + 1; j < pozycje.size(); ++j) {
            if (sekwencja_bialka[j] != 'H') continue;
            if (std::abs(static_cast<int>(i) - static_cast<int>(j)) == 1) continue;
            if (odleglosc(pozycje[i], pozycje[j]) == 1) {
                energia -= 1.0;
            }
        }
    }
    return energia;
}

/**
 * Sprawdza, czy dane pole jest wolne (niezajęte przez aminokwas).
 */
bool HP_model::pole_wolne(const Vec3& pos) const {
    return mapa_pozycji.find(pos) == mapa_pozycji.end();
}

/**
 * RADYKALNIE PRZEPROJEKTOWANA funkcja przesunięcia końca.
 * Bezpośrednio wyszukuje wszystkie wolne pozycje sąsiadujące z sąsiadem końca.
 */
std::vector<Vec3> HP_model::ruch_przesun_koniec() {
    const std::vector<Vec3> kierunki = {
        Vec3{1,0,0}, Vec3{-1,0,0}, Vec3{0,1,0}, Vec3{0,-1,0}, Vec3{0,0,1}, Vec3{0,0,-1}
    };
    
    std::vector<std::pair<size_t, Vec3>> mozliwe_ruchy;
    
    // Sztuczne generowanie ruchów pierwszego aminokwasu
    if (pozycje.size() > 1) {
        size_t indeks = 0;
        Vec3 drugi = pozycje[1]; // Drugi aminokwas (pozostaje na miejscu)
        
        // Szukamy wszystkich wolnych pozycji sąsiadujących z drugim aminokwasem
        for (const auto& dir : kierunki) {
            Vec3 kandydat = drugi + dir;
            
            // Sprawdź, czy kandydat jest wolny i nie jest aktualną pozycją pierwszego aminokwasu
            if (kandydat != pozycje[0] && pole_wolne(kandydat)) {
                std::cout << "ZNALEZIONO ruch końca dla pierwszego aminokwasu!" << std::endl;
                mozliwe_ruchy.push_back({indeks, kandydat});
            }
        }
    }
    
    // Sztuczne generowanie ruchów ostatniego aminokwasu
    if (pozycje.size() > 1) {
        size_t indeks = pozycje.size() - 1;
        Vec3 przedostatni = pozycje[indeks - 1]; // Przedostatni aminokwas (pozostaje na miejscu)
        
        // Szukamy wszystkich wolnych pozycji sąsiadujących z przedostatnim aminokwasem
        for (const auto& dir : kierunki) {
            Vec3 kandydat = przedostatni + dir;
            
            // Sprawdź, czy kandydat jest wolny i nie jest aktualną pozycją ostatniego aminokwasu
            if (kandydat != pozycje[indeks] && pole_wolne(kandydat)) {
                std::cout << "ZNALEZIONO ruch końca dla ostatniego aminokwasu!" << std::endl;
                mozliwe_ruchy.push_back({indeks, kandydat});
            }
        }
    }
    
    std::cout << "Możliwych ruchów końca: " << mozliwe_ruchy.size() << std::endl;
    
    if (!mozliwe_ruchy.empty()) {
        std::uniform_int_distribution<> rand_idx(0, mozliwe_ruchy.size() - 1);
        auto wybrany_ruch = mozliwe_ruchy[rand_idx(gen)];
        
        std::vector<Vec3> nowa_konformacja = pozycje;
        nowa_konformacja[wybrany_ruch.first] = wybrany_ruch.second;
        return nowa_konformacja;
    }
    
    nieudane_koniec++;
    return {};
}

/**
 * Obrót narożnika: losowo wybierz możliwy ruch narożnika.
 */
std::vector<Vec3> HP_model::ruch_obrot_naroznika() {
    std::vector<Vec3> mozliwe_ruchy = {
        Vec3{1,0,0}, Vec3{-1,0,0}, Vec3{0,1,0}, Vec3{0,-1,0}, Vec3{0,0,1}, Vec3{0,0,-1}
    };
    std::vector<std::pair<size_t, Vec3>> mozliwosci; // (indeks, nowa_pozycja)

    for (size_t i = 1; i < pozycje.size()-1; ++i) {
        Vec3 prev = pozycje[i-1];
        Vec3 curr = pozycje[i];
        Vec3 next = pozycje[i+1];
        
        // Sprawdź, czy aminokwas jest w narożniku (sąsiedzi nie leżą w linii prostej)
        if (odleglosc(prev, next) == 2) {
            // Znajdź wszystkie możliwe nowe pozycje dla narożnika
            for (const auto& dir : mozliwe_ruchy) {
                Vec3 kandydat = prev + dir;
                // Kandydat musi być: wolny, różny od curr, połączony z prev i next
                if (kandydat != curr && pole_wolne(kandydat) && 
                    sa_sasiadami(kandydat, prev) && sa_sasiadami(kandydat, next)) {
                    mozliwosci.push_back({i, kandydat});
                }
            }
        }
    }

    std::cout << "Możliwych ruchów narożnika: " << mozliwosci.size() << std::endl;

    if (!mozliwosci.empty()) {
        std::uniform_int_distribution<> rand_moz(0, static_cast<int>(mozliwosci.size())-1);
        auto wybor = mozliwosci[rand_moz(gen)];
        
        std::vector<Vec3> nowa_pozycja = pozycje;
        nowa_pozycja[wybor.first] = wybor.second;
        return nowa_pozycja;
    }

    nieudane_naroznik++;
    return {};
}

/**
 * Crankshaft: obracanie dwóch kolejnych aminokwasów.
 */
std::vector<Vec3> HP_model::ruch_crankshaft() {
    std::vector<Vec3> kierunki = {
        Vec3{1,0,0}, Vec3{-1,0,0}, Vec3{0,1,0}, Vec3{0,-1,0}, Vec3{0,0,1}, Vec3{0,0,-1}
    };
    
    struct Ruch {
        size_t indeks_i;
        Vec3 nowa_poz_i_plus_1;
        Vec3 nowa_poz_i_plus_2;
    };
    
    std::vector<Ruch> mozliwe_ruchy;
    
    // Szukamy fragmentów 4 aminokwasów
    for (size_t i = 0; i + 3 < pozycje.size(); ++i) {
        Vec3 a = pozycje[i];      // pierwszy aminokwas (stały)
        Vec3 b = pozycje[i+1];    // drugi aminokwas (do przesunięcia)
        Vec3 c = pozycje[i+2];    // trzeci aminokwas (do przesunięcia)
        Vec3 d = pozycje[i+3];    // czwarty aminokwas (stały)
        
        // W siatce 3D, odległość Manhattan między a i d może być 2 lub 3 dla crankshaft
        if (odleglosc(a, d) != 2 && odleglosc(a, d) != 3) continue;
        
        // Generujemy wszystkie możliwe nowe pozycje dla b i c
        for (const auto& dir_b : kierunki) {
            Vec3 nowe_b = a + dir_b;
            
            // Warunki dla nowego b
            if (nowe_b == b || nowe_b == c || nowe_b == d || 
                !pole_wolne(nowe_b) || !sa_sasiadami(nowe_b, a)) {
                continue;
            }
            
            for (const auto& dir_c : kierunki) {
                Vec3 nowe_c = d + dir_c;
                
                // Warunki dla nowego c
                if (nowe_c == a || nowe_c == b || nowe_c == c || nowe_c == nowe_b || 
                    !pole_wolne(nowe_c) || !sa_sasiadami(nowe_c, d) || !sa_sasiadami(nowe_c, nowe_b)) {
                    continue;
                }
                
                mozliwe_ruchy.push_back({i, nowe_b, nowe_c});
            }
        }
    }
    
    std::cout << "Możliwych ruchów crankshaft: " << mozliwe_ruchy.size() << std::endl;
    
    if (!mozliwe_ruchy.empty()) {
        std::uniform_int_distribution<> rand_idx(0, mozliwe_ruchy.size() - 1);
        auto wybrany_ruch = mozliwe_ruchy[rand_idx(gen)];
        
        std::vector<Vec3> nowa_konformacja = pozycje;
        nowa_konformacja[wybrany_ruch.indeks_i + 1] = wybrany_ruch.nowa_poz_i_plus_1;
        nowa_konformacja[wybrany_ruch.indeks_i + 2] = wybrany_ruch.nowa_poz_i_plus_2;
        return nowa_konformacja;
    }
    
    nieudane_crankshaft++;
    return {};
}

/**
 * Algorytm Metropolisa z symulowanym wyżarzaniem.
 */
void HP_model::algorytm_metropolisa(double T0, double T_inf, double alpha, int steps) {
    double T = T0;
    std::ofstream energy_file("energia.txt");
    std::ofstream traj_file("trajektoria.txt");
    std::ofstream koniec_file("koncowa_konformacja.txt");

    double energia_poczatkowa = oblicz_energie();
    std::cout << "Energia początkowa: " << energia_poczatkowa << std::endl;

    // Losowy wybór rodzaju ruchu
    std::uniform_int_distribution<> dist_typ_ruchu(0, 2);

    for (int step = 0; step < steps; ++step) {
        int typ_ruchu = dist_typ_ruchu(gen);
        std::vector<Vec3> nowa_konformacja;

        if (typ_ruchu == 0) {
            ++proponowane_koniec;
            nowa_konformacja = ruch_przesun_koniec();
        } else if (typ_ruchu == 1) {
            ++proponowane_naroznik;
            nowa_konformacja = ruch_obrot_naroznika();
        } else {
            ++proponowane_crankshaft;
            nowa_konformacja = ruch_crankshaft();
        }

        if (!nowa_konformacja.empty()) {
            double E_old = oblicz_energie();
            
            // Zapamiętaj starą pozycję na wypadek odrzucenia ruchu
            auto kopia_pozycji = pozycje;
            
            // Zaktualizuj pozycje i mapę pozycji
            pozycje = nowa_konformacja;
            mapa_pozycji.clear();
            for (size_t i = 0; i < pozycje.size(); ++i) {
                mapa_pozycji[pozycje[i]] = sekwencja_bialka[i];
            }
            
            double E_new = oblicz_energie();
            
            // POPRAWIONA formuła akceptacji Metropolisa
            if (E_new <= E_old || 
                std::uniform_real_distribution<>(0,1)(gen) < std::exp(-(E_new-E_old)/T)) {
                // Ruch zaakceptowany
                if (typ_ruchu == 0) ++zaakceptowane_koniec;
                else if (typ_ruchu == 1) ++zaakceptowane_naroznik;
                else ++zaakceptowane_crankshaft;
            } else {
                // Ruch odrzucony - przywróć poprzednią konformację
                pozycje = kopia_pozycji;
                mapa_pozycji.clear();
                for (size_t i = 0; i < pozycje.size(); ++i) {
                    mapa_pozycji[pozycje[i]] = sekwencja_bialka[i];
                }
            }
        }
        
        // Zapisz energię i pozycje do plików
        energy_file << oblicz_energie() << "\n";
        for (const auto& poz : pozycje) {
            traj_file << poz.x << " " << poz.y << " " << poz.z << " ";
        }
        traj_file << "\n";
        
        // Schładzanie temperatury (symulowane wyżarzanie)
        T = std::max(alpha*T, T_inf);
        
        // Co 1000 kroków wypisz informację o postępie
        if (step % 1000 == 0) {
            std::cout << "Krok " << step << ", temperatura: " << T 
                      << ", energia: " << oblicz_energie() << std::endl;
        }
    }
    
    // Zapisz końcową konformację
    for (const auto& poz : pozycje) {
        koniec_file << poz.x << " " << poz.y << " " << poz.z << "\n";
    }
    
    energy_file.close();
    traj_file.close();
    koniec_file.close();
    
    std::cout << "Energia końcowa: " << oblicz_energie() << std::endl;
}

/**
 * Wypisuje statystyki ruchów po zakończeniu symulacji.
 */
void HP_model::wypisz_statystyki() const {
    std::cout << "Przesunięcia końca: proponowane " << proponowane_koniec
              << ", zaakceptowane " << zaakceptowane_koniec
              << ", nieudane próby: " << nieudane_koniec << "\n";
    std::cout << "Obroty narożnika: proponowane " << proponowane_naroznik
              << ", zaakceptowane " << zaakceptowane_naroznik
              << ", nieudane próby: " << nieudane_naroznik << "\n";
    std::cout << "Obroty crankshaft: proponowane " << proponowane_crankshaft
              << ", zaakceptowane " << zaakceptowane_crankshaft
              << ", nieudane próby: " << nieudane_crankshaft << "\n";
}
