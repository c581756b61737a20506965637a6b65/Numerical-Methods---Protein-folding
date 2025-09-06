# Protein Folding – HP Lattice Model in 3D

This repository contains a protein folding simulator based on the HP lattice model using the Metropolis algorithm with simulated annealing. The project was developed for the *Numerical Methods* course and aims to find low-energy 3D conformations of the **ubiquitin** protein using a simplified, discrete model.

---

## 🧰 Dependencies

### C++ Side (core simulation):
- **GCC** – C++ compiler
- **CMake** – Build system
- **GDB** – Optional debugger
- **Nix** – Reproducible environment (`flake.nix`)

### Python Side (visualization & plots):
- `matplotlib`
- `numpy`
- `os`, `sys`, `csv` – standard libraries

---

## 📁 Project Structure

- `Vec3.*` – 3D vector operations and coordinate hashing
- `HP_model.*` – Folding logic: energy calculation, conformational moves, Metropolis sampling
- `Main.cpp` – Entry point; runs the simulation and exports output files
- `CMakeLists.txt` – Build configuration
- `Out/` or `./` – Output files (plots, final structure, animation)
- `plot_results.py` – Python script for plotting energy and rendering final conformation

---

## ⚙️ How to Build & Run

### 1. Build the project
```bash
nix develop           # optional, if using Nix
cmake -S . -B build
cmake --build build
