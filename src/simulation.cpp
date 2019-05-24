#include "../include/simulation.h"

Simulation::Simulation() {}

Simulation::Simulation(int nLin, int nCol) {
        // prepare_petri(nLin, nCol);
        num_rows = nLin + 2;  // Two more rows to prevent error.
        num_col = nCol + 2;   // Two more col to prevent error.

        petri_dish.resize(num_rows);  // Define the number of lines

        // In every row, create n columns with -1 (indicate empty space),
        // thus this is equivalent to a num_rows x num_col.
        for (int i = 0; i < num_rows; i++) {
                petri_dish[i].assign(num_col, dead);
        }
}

Simulation::~Simulation() {}

void Simulation::prepare_petri(int size_row, int size_col) {
        num_rows = size_row + 2;  // Two more rows to prevent error.
        num_col = size_col + 2;   // Two more col to prevent error.

        petri_dish.resize(num_rows);  // Define the number of lines

        // In every row, create n columns with -1 (indicate empty space),
        // thus this is equivalent to a num_rows x num_col.
        for (int i = 0; i < num_rows; i++) {
                petri_dish[i].assign(num_col, dead);
        }
}

int Simulation::surroundings(int i, int j) {
        int n = 0;  // Number of neighbors

        if ((i > getNumRows()) || (j > getNumCol())) {
                std::cerr << "\n\033[0;31m>>> Error: out of range!\033[0m\n";
                exit(EXIT_FAILURE);
        }

        if (petri_dish[i - 1][j - 1] == alive) {
                n++;
        }

        if (petri_dish[i - 1][j] == alive) {
                n++;
        }

        if (petri_dish[i - 1][j + 1] == alive) {
                n++;
        }

        if (petri_dish[i][j - 1] == alive) {
                n++;
        }

        if (petri_dish[i][j + 1] == alive) {
                n++;
        }

        if (petri_dish[i + 1][j - 1] == alive) {
                n++;
        }

        if (petri_dish[i + 1][j] == alive) {
                n++;
        }

        if (petri_dish[i + 1][j + 1] == alive) {
                n++;
        }

        return n;
}

void Simulation::set_alive() {
        log_struct current_gen;  // Save the coordinates from this generation.
        int idx_cell = 0;        // index of each cell (struct).

        // First generation after read the file. Don't apply rules.
        if (num_gen == 0) {
                // Copy living cells from petri_dish to log of generation.
                for (int i = 1; i <= getNumRows(); i++) {
                        for (int j = 1; j <= getNumCol(); j++) {
                                // Create and store the coords of living cells.
                                if (petri_dish[i][j] == alive) {
                                        current_gen.push_back(Cell());
                                        current_gen[idx_cell].x = i;
                                        current_gen[idx_cell].y = j;
                                        idx_cell++;
                                }
                        }
                }
        } else {  // Apply rules to define cell status.

                // Copy living cells from petri_dish to log of generation.
                for (int i = 1; i <= getNumRows(); i++) {
                        for (int j = 1; j <= getNumCol(); j++) {
                                if (surroundings(i, j) == 3) {
                                        current_gen.push_back(Cell());
                                        current_gen[idx_cell].x = i;
                                        current_gen[idx_cell].y = j;
                                        idx_cell++;

                                } else if (petri_dish[i][j] == alive) {
                                        if ((surroundings(i, j) <= 3) && (surroundings(i, j) >= 2)) {
                                                current_gen.push_back(Cell());
                                                current_gen[idx_cell].x = i;
                                                current_gen[idx_cell].y = j;
                                                idx_cell++;
                                        }
                                }
                        }
                }
        }
        log_master.push_back(current_gen);  // Push to the log_master the current generation.
}
