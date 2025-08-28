# Numerical-Methods---Protein-folding

This program was developed as a project for the Numerical Methods course. It simulates simplified protein folding using the HP lattice model in 3D space. The folding process is modeled using the Metropolis algorithm with simulated annealing to find a conformation with minimal energy.  Assumptions and simplifications:  Proteins are represented as a sequence of two types of amino acids: H (hydrophobic) and P (polar).  The simulation runs on a 3D regular lattice — amino acids occupy discrete positions in space.  Only H-H non-covalent neighboring contacts contribute to energy (−1 per contact).  Three types of moves are implemented: end move, corner rotation, and crankshaft move.  Energy is minimized using Monte Carlo sampling with temperature-based acceptance.  Boltzmann constant is set to 1.  Protein folding starts from either a linear or random conformation.

# How the Program Works (Implementation Details)

(To be completed – e.g. programming language, structure, main functions, data structures used, visualization tools, etc.)
