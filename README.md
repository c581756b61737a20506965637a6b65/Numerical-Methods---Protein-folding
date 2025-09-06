# Protein Folding – HP Lattice Model in 3D

This repository contains a protein folding simulator based on the HP lattice model using the Metropolis algorithm with simulated annealing. The project was developed for the *Numerical Methods* course and focuses on predicting a low-energy 3D structure of the **ubiquitin** protein on a discrete cubic lattice.

---

## 🧰 Dependencies

- **GCC** – C++ compiler
- **CMake** – Build system
- **GDB** – Optional debugger
- **Nix** – Reproducible environment (`flake.nix` provided)

---

## 📁 Project Structure

- `Vec3.*` – 3D coordinate representation with vector operations and hashing
- `HP_model.*` – Core logic: folding simulation, energy evaluation, conformational moves
- `Main.cpp` – Entry point: initializes simulation and outputs the result
- `CMakeLists.txt` – Build configuration
- `Out/` – Output folder with plots and structure visualization

---

## ⚙️ How to Build & Run

```bash
nix develop           # optional, sets up the environment
cmake -S . -B build
cmake --build build
./build/ProteinFolding


