#include "../include/simulation.h"

void Simulation::initialize(int argc, char *argv[]) {
        // Read options from cli.
        if (read_options(argc, argv) == -1) {
                exit(EXIT_FAILURE);
        }

        read_file();  // Read the config file.

        if (options.maxgen == int_size) {
                std::cerr << ">>> Running simulation until extinction/stability is reached.\n";
        } else {
                std::cerr
                    << ">>> Running simulation up to " << options.maxgen
                    << " generations, or until extinction/stability is reached, whichever comes first.\n";
        }

        print_initial_msg();  // Print welcome message.
        set_alive();          // Set living cells.
}

bool Simulation::game_over() {
        // Check if the number of generations reached the max.
        if (num_gen >= (options.maxgen - 1)) {
                return true;
        }

        // Check if the current generation is extinct or stable.
        if (extinct() || stable()) {
                return true;
        }

        return false;  // If the game will continue.
}

void Simulation::process_events() {
        ++num_gen;    // Upgrade the number of generations.
        set_alive();  // Set living cells.
}

void Simulation::update() {
        auto prev_gen = (num_gen - 1);  // Previous generation.

        // Search by cell in the previous generation.
        // If not found, kill cell (yeah!).
        for (int i = 0; i < (int)log_master[prev_gen].size(); i++) {
                auto idx_x_prev = log_master[prev_gen][i].x;
                auto idx_y_prev = log_master[prev_gen][i].y;
                bool found = false;

                for (int j = 0; j < (int)log_master[num_gen].size(); j++) {
                        auto idx_x_curr = log_master[num_gen][j].x;
                        auto idx_y_curr = log_master[num_gen][j].y;

                        if ((idx_x_prev == idx_x_curr) && (idx_y_prev == idx_y_curr)) {
                                found = true;
                                break;
                        }
                }

                // Kill cell.
                if (!found) {
                        petri_dish[idx_x_prev][idx_y_prev] = dead;
                }
        }

        // Define the living cells.
        for (int i = 0; i < (int)log_master.size(); i++) {
                for (int j = 0; j < (int)log_master[num_gen].size(); j++) {
                        auto idx_x = log_master[num_gen][j].x;
                        auto idx_y = log_master[num_gen][j].y;

                        petri_dish[idx_x][idx_y] = alive;
                }
        }
}

void Simulation::render() {
        // Print the index of the current generation.
        if (options.maxgen == int_size) {
                std::cout << "Generation [" << (num_gen + 1) << "]:["
                          << "\u221E]" << std::endl;
        } else {
                std::cout << "Generation [" << (num_gen + 1) << "]:[" << options.maxgen << "]" << std::endl;
        }

        print_petri();  // Show the petri_dish.
        std::cout << std::endl;
}

bool Simulation::extinct() {
        // Verify if not exists living cells from log.
        if (log_master[num_gen].size() == 0) {
                std::cerr << "\033[0;31m>>> Simulation ended due to extinction. \033[0m\n\n";
                return true;
        } else {
                return false;
        }
}

bool Simulation::stable() {
        // Verify if the current generation is equal to a previous generation.
        for (auto i = 0; i < num_gen; i++) {  
                bool equal = true;
                
                for (auto j = 0; j < (int)log_master[num_gen].size(); j++) {       
                        if (log_master[i].size() == log_master[num_gen].size()) {  
                                if ((log_master[i][j].x != log_master[num_gen][j].x) &&
                                    (log_master[i][j].y != log_master[num_gen][j].y)) {
                                        equal = false;
                                        break;
                                }

                        } else {
                                equal = false;
                                break;
                        }
                }

                if (equal) {
                        std::cerr << "\033[0;31m>>> Simulation ended due to stability. \033[0m\n";
                        std::cerr << "\033[0;31m>>> Generation [" << (i + 1) << "] equals to [" << num_gen + 1
                                  << "]. \033[0m\n\n";
                        return true;
                }
        }

        return false;
}