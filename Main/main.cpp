#include "../Header/HP_model.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <filesystem>

// Struktura do przechowywania parametrów symulacji
struct ParametrSymulacji {
    double T0;           // temperatura początkowa
    double T_inf;        // temperatura końcowa
    double alpha;        // współczynnik chłodzenia
    int kroki;           // liczba kroków
    bool losowa_init;    // sposób inicjalizacji (true = losowa, false = linia prosta)
    
    // Konstruktor
    ParametrSymulacji(double t0, double t_inf, double a, int k, bool los) 
        : T0(t0), T_inf(t_inf), alpha(a), kroki(k), losowa_init(los) {}
};

// Funkcja sprawdzająca i tworząca katalog Out, jeśli nie istnieje
void zapewnij_katalog_out() {
    namespace fs = std::filesystem;
    if (!fs::exists("Out")) {
        fs::create_directory("Out");
        std::cout << "Utworzono katalog Out/ dla plików wynikowych." << std::endl;
    }
}

// Funkcja kopiująca pliki z katalogu głównego do katalogu Out
void kopiuj_do_out(const std::string& nazwa_pliku) {
    namespace fs = std::filesystem;
    if (fs::exists(nazwa_pliku)) {
        fs::copy_file(nazwa_pliku, "Out/" + nazwa_pliku, fs::copy_options::overwrite_existing);
        std::cout << "Skopiowano " << nazwa_pliku << " do katalogu Out/" << std::endl;
    } else {
        std::cerr << "Błąd: Plik " << nazwa_pliku << " nie istnieje i nie może być skopiowany." << std::endl;
    }
}

// Funkcja przeprowadzająca pojedynczą symulację z danymi parametrami
void uruchom_symulacje(const ParametrSymulacji& params, std::ofstream& wyniki_plik) {
    // Inicjalizacja modelu HP - używamy konstruktora bezparametrowego
    HP_model model;
    
    // Próbujemy wygenerować początkową konformację
    int proby = 0;
    const int max_proby = 1000;
    bool sukces = false;
    
    while (!sukces && proby < max_proby) {
        sukces = model.generuj_startowa_konformacje(params.losowa_init);
        proby++;
    }
    
    if (!sukces) {
        std::cerr << "Nie udało się wygenerować początkowej konformacji po " << max_proby << " próbach!" << std::endl;
        return;
    }
    
    // Uruchamiamy symulację
    model.algorytm_metropolisa(params.T0, params.T_inf, params.alpha, params.kroki);
    
    // Obliczamy końcową energię
    double energia_koncowa = model.oblicz_energie();
    
    // Pobieramy statystyki akceptowanych ruchów
    int akceptowane_koniec = model.get_zaakceptowane_koniec();
    int akceptowane_naroznik = model.get_zaakceptowane_naroznik();
    int akceptowane_crankshaft = model.get_zaakceptowane_crankshaft();
    
    // Kopiujemy pliki wynikowe do katalogu Out
    kopiuj_do_out("energia.txt");
    kopiuj_do_out("trajektoria.txt");
    kopiuj_do_out("koncowa_konformacja.txt");
    
    // Zapisujemy wyniki do pliku CSV
    wyniki_plik << (params.losowa_init ? "losowa" : "liniowa") << ","
                << params.T0 << ","
                << params.T_inf << ","
                << params.alpha << ","
                << params.kroki << ","
                << energia_koncowa << ","
                << akceptowane_koniec << ","
                << akceptowane_naroznik << ","
                << akceptowane_crankshaft << "\n";
    
    // Wyświetlamy wyniki na ekranie
    std::cout << std::setw(15) << (params.losowa_init ? "losowa" : "liniowa")
              << std::setw(10) << params.T0
              << std::setw(10) << params.T_inf
              << std::setw(10) << params.alpha
              << std::setw(10) << params.kroki
              << std::setw(10) << energia_koncowa
              << std::setw(10) << akceptowane_koniec
              << std::setw(15) << akceptowane_naroznik
              << std::setw(15) << akceptowane_crankshaft << std::endl;
}

// Funkcja przeprowadzająca serię testów z różnymi parametrami
void przeprowadz_testy() {
    // Zapewniamy, że katalog Out istnieje
    zapewnij_katalog_out();
    
    // Otwieramy plik do zapisywania wyników
    std::ofstream wyniki_plik("Out/wyniki_testow.csv");
    
    // Nagłówek pliku CSV
    wyniki_plik << "Inicjalizacja,T0,T_inf,alpha,Kroki,Energia,Akc_end,Akc_corner,Akc_crankshaft\n";
    
    // Nagłówek tabeli w konsoli
    std::cout << std::setw(15) << "Inicjalizacja" 
              << std::setw(10) << "T0" 
              << std::setw(10) << "T_inf" 
              << std::setw(10) << "alpha" 
              << std::setw(10) << "Kroki" 
              << std::setw(10) << "Energia" 
              << std::setw(10) << "Akc_end" 
              << std::setw(15) << "Akc_corner" 
              << std::setw(15) << "Akc_crankshaft" << std::endl;
    
    std::cout << std::string(105, '-') << std::endl;
    
    // Dla obu metod inicjalizacji - liniowej i losowej
    for (bool losowa_init : {false, true}) {
        // Test 1: Parametry domyślne
        std::cout << "Test 1: Parametry domyślne, inicjalizacja " 
                  << (losowa_init ? "losowa" : "liniowa") << std::endl;
        uruchom_symulacje(ParametrSymulacji(10.0, 1.0, 0.999, 10000, losowa_init), wyniki_plik);
        
        // Test 2: Niska temperatura początkowa
        std::cout << "Test 2: Niska temperatura początkowa, inicjalizacja " 
                  << (losowa_init ? "losowa" : "liniowa") << std::endl;
        uruchom_symulacje(ParametrSymulacji(5.0, 1.0, 0.999, 10000, losowa_init), wyniki_plik);
        
        // Test 3: Wysoka temperatura początkowa
        std::cout << "Test 3: Wysoka temperatura początkowa, inicjalizacja " 
                  << (losowa_init ? "losowa" : "liniowa") << std::endl;
        uruchom_symulacje(ParametrSymulacji(20.0, 1.0, 0.999, 10000, losowa_init), wyniki_plik);
        
        // Test 4: Niska temperatura końcowa
        std::cout << "Test 4: Niska temperatura końcowa, inicjalizacja " 
                  << (losowa_init ? "losowa" : "liniowa") << std::endl;
        uruchom_symulacje(ParametrSymulacji(10.0, 0.5, 0.999, 10000, losowa_init), wyniki_plik);
        
        // Test 5: Wysoka temperatura końcowa
        std::cout << "Test 5: Wysoka temperatura końcowa, inicjalizacja " 
                  << (losowa_init ? "losowa" : "liniowa") << std::endl;
        uruchom_symulacje(ParametrSymulacji(10.0, 2.0, 0.999, 10000, losowa_init), wyniki_plik);
        
        // Test 6: Szybkie schładzanie
        std::cout << "Test 6: Szybkie schładzanie, inicjalizacja " 
                  << (losowa_init ? "losowa" : "liniowa") << std::endl;
        uruchom_symulacje(ParametrSymulacji(10.0, 1.0, 0.995, 10000, losowa_init), wyniki_plik);
        
        // Test 7: Wolne schładzanie
        std::cout << "Test 7: Wolne schładzanie, inicjalizacja " 
                  << (losowa_init ? "losowa" : "liniowa") << std::endl;
        uruchom_symulacje(ParametrSymulacji(10.0, 1.0, 0.9999, 10000, losowa_init), wyniki_plik);
        
        // Test 8: Mała liczba kroków
        std::cout << "Test 8: Mała liczba kroków, inicjalizacja " 
                  << (losowa_init ? "losowa" : "liniowa") << std::endl;
        uruchom_symulacje(ParametrSymulacji(10.0, 1.0, 0.999, 5000, losowa_init), wyniki_plik);
        
        // Test 9: Duża liczba kroków
        std::cout << "Test 9: Duża liczba kroków, inicjalizacja " 
                  << (losowa_init ? "losowa" : "liniowa") << std::endl;
        uruchom_symulacje(ParametrSymulacji(10.0, 1.0, 0.999, 20000, losowa_init), wyniki_plik);
    }
    
    wyniki_plik.close();
    std::cout << "Wyniki zostały zapisane do pliku 'Out/wyniki_testow.csv'" << std::endl;
}

// Funkcja testująca najlepsze parametry i zapisująca historię energii
void testuj_najlepsze_parametry() {
    // Zapewniamy, że katalog Out istnieje
    zapewnij_katalog_out();
    
    std::cout << "Testowanie najlepszych parametrów na podstawie analizy wyników..." << std::endl;
    
    // Inicjalizacja modelu z domyślną sekwencją
    HP_model model;
    
    if (model.generuj_startowa_konformacje(true)) {  // inicjalizacja losowa
        // Najlepsze parametry na podstawie wyników testów
        double T0 = 10.0;    // temperatura początkowa
        double T_inf = 0.5;  // temperatura końcowa
        double alpha = 0.999; // współczynnik chłodzenia
        int kroki = 10000;   // liczba kroków
        
        // Uruchamiamy algorytm Metropolisa
        model.algorytm_metropolisa(T0, T_inf, alpha, kroki);
        
        // Kopiujemy pliki wynikowe do katalogu Out
        kopiuj_do_out("energia.txt");
        kopiuj_do_out("trajektoria.txt");
        kopiuj_do_out("koncowa_konformacja.txt");
        
        // Wyświetlamy statystyki
        model.wypisz_statystyki();
        
        std::cout << "Historia energii zapisana do pliku 'Out/energia.txt'" << std::endl;
        std::cout << "Końcowa konformacja zapisana do pliku 'Out/koncowa_konformacja.txt'" << std::endl;
    } else {
        std::cerr << "Nie udało się wygenerować początkowej konformacji!" << std::endl;
    }
}

// Dodanie informacji o autorze i dacie kompilacji
void wyswietl_informacje() {
    std::cout << "=== Program do symulacji zwijania białek w modelu HP ===" << std::endl;
    std::cout << "Data utworzenia: 2025-09-06 12:30:58 UTC" << std::endl;
    std::cout << "Autor: lpieczke" << std::endl;
    std::cout << "==========================================================" << std::endl;
}

int main() {
    // Wyświetl informacje o programie
    wyswietl_informacje();
    
    // Uruchamiamy systematyczne testy parametrów
    przeprowadz_testy();
    
    // Przeprowadzamy szczegółową symulację dla najlepszych parametrów
    testuj_najlepsze_parametry();
    
    // Informacja o uruchomieniu skryptów
    std::cout << "\nAby wygenerować wykresy i wizualizacje, uruchom skrypty z katalogu Python/:" << std::endl;
    std::cout << "python ../Python/plot_energy.py" << std::endl;
    std::cout << "python ../Python/animate_folding.py" << std::endl;
    
    return 0;
}
