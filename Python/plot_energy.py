import matplotlib.pyplot as plt
import numpy as np
import os

# Ścieżka do pliku energia.txt
file_path = '../Out/energia.txt'
if not os.path.exists(file_path):
    file_path = 'energia.txt'  # Próbuj lokalnie jako alternatywę

# Wczytaj dane z pliku energia.txt
try:
    energy_data = np.loadtxt(file_path)
    
    # Utwórz oś x (krok symulacji)
    steps = np.arange(len(energy_data))
    
    # Stwórz wykres
    plt.figure(figsize=(10, 6))
    plt.plot(steps, energy_data, 'b-')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.xlabel('Krok symulacji')
    plt.ylabel('Energia')
    plt.title('Przebieg energii podczas symulacji zwijania białka')
    
    # Ścieżka do zapisania plików wynikowych
    output_dir = '../Out'
    if not os.path.exists(output_dir):
        output_dir = '.'
    
    # Zapisz wykres
    plt.savefig(f'{output_dir}/energia_wykres.pdf', bbox_inches='tight')
    plt.savefig(f'{output_dir}/energia_wykres.png', bbox_inches='tight', dpi=300)
    plt.close()
    
    print(f"Wykres energii zapisany w katalogu {output_dir} jako energia_wykres.pdf i energia_wykres.png")
except Exception as e:
    print(f"Błąd generowania wykresu energii: {e}")
    print("Upewnij się, że plik energia.txt istnieje w katalogu Out/ lub bieżącym katalogu.")
