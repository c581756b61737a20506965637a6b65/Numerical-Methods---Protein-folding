import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import os

# Ścieżka do pliku koncowa_konformacja.txt
file_path = '../Out/koncowa_konformacja.txt'
if not os.path.exists(file_path):
    file_path = 'koncowa_konformacja.txt'  # Próbuj lokalnie jako alternatywę

# Ścieżka do zapisania plików wynikowych
output_dir = '../Out'
if not os.path.exists(output_dir):
    output_dir = '.'

# Próbujemy wizualizować końcową konformację
try:
    if os.path.exists(file_path):
        positions = np.loadtxt(file_path)
        
        fig = plt.figure(figsize=(8, 8))
        ax = fig.add_subplot(111, projection='3d')
        
        x = positions[:, 0]
        y = positions[:, 1]
        z = positions[:, 2]
        
        ax.plot(x, y, z, 'o-', color='blue', linewidth=2)
        
        # Sekwencja ubikwityny w modelu HP
        sekwencja_hp = "PHPHHHHHPHPHPHHPPPPPHPPPPHHHPPPPPHPPPHHPHPHHHHPPPPHHHHPPHPHPHHHHHHHHHPPHHPP"
        
        # Kolorowanie hydrofobowych (H) i polarnych (P) aminokwasów
        colors = ['red' if aa == 'H' else 'green' for aa in sekwencja_hp[:len(x)]]
        ax.scatter(x, y, z, color=colors, s=100)
        
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')
        ax.set_title('Końcowa konformacja białka')
        
        plt.savefig(f'{output_dir}/zwijanie.png', bbox_inches='tight', dpi=300)
        print(f"Zapisano obraz konformacji jako {output_dir}/zwijanie.png")
    else:
        print(f"Błąd: Plik {file_path} nie istnieje")
except Exception as e:
    print(f"Błąd wizualizacji końcowej konformacji: {e}")
