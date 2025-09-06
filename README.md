# Numerical-Methods---Protein-folding

This program was developed as a project for the Numerical Methods course. It simulates simplified protein folding using the HP lattice model in 3D space. The folding process is modeled using the Metropolis algorithm with simulated annealing to find a conformation with minimal energy.

## Dependencies

The project requires the following tools and libraries to build and run:
- **CMake**: For managing the build process.
- **GCC**: The GNU Compiler Collection for compiling the code.
- **GDB**: GNU Debugger for debugging (optional but recommended).
- **Nix**: For reproducible development environments (flake configuration provided).

## Project Structure and Components

The project is divided into the following main components:

1. **Vec3**:
   - A lightweight structure to represent 3D coordinates of amino acids on the lattice.
   - Provides utility functions for vector operations and hashing, enabling efficient use in data structures.

2. **HP_model**:
   - Implements the core simulation logic for the HP lattice model.
   - Handles the initialization of protein conformations, energy calculations, and the implementation of moves (end move, corner rotation, crankshaft move).
   - Encapsulates the Metropolis algorithm and simulated annealing process for energy minimization.

3. **Main**:
   - The entry point of the program.
   - Invokes the `HP_model` to initialize the protein, run the folding simulation, and save the final conformation to a text file.
   - Designed for straightforward execution and output analysis.

4. **CMakeLists.txt**:
   - Configures the build process for the project.
   - Ensures that all dependencies are linked and compiles the source code into an executable.

## How to Build and Run

1. Use `nix develop` to set up the development environment.
2. Run `cmake -S . -B build` to generate build files.
3. Execute `cmake --build build` to compile the source code.
4. Run the program using `./build/ProteinFolding`.

The output will be saved in the `koncowa_konformacja.txt` file, where each line represents the 3D coordinates of consecutive amino acids in the folded protein.

