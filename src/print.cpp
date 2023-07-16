#include "../include/simulation.h"

void Simulation::print_options() {
    std::cout << "[Options]\n\n";
    std::cout << "imgdir: \"" << options.imgdir << "\"" << std::endl;
    std::cout << "maxgen: " << options.maxgen << std::endl;
    std::cout << "fps: " << options.fps << std::endl;
    std::cout << "blocksize: " << options.blocksize << std::endl;
    std::cout << "bkgcolor: " << options.bkgcolor << std::endl;
    std::cout << "alivecolor: " << options.alivecolor << std::endl;
    std::cout << "inputfile: \"" << options.inputfile << "\"" << std::endl;
    std::cout << "outfile: '\"" << options.outfile << "\"" << std::endl;
}

void Simulation::print_matrix() {
    std::ostringstream data;

    std::cout << "[" << num_rows << " x " << num_col << "]\n" << std::endl;
    for (int i = 0; i < num_rows; i++) {
        if (i < 10) {
            data << "[" << i << "]" << std::setw(2) << " ";
        } else {
            data << "[" << i << "]" << std::setw(1) << " ";
        }

        for (int j = 0; j < num_col; j++) {
            data << std::setw(1) << petri_dish[i][j] << " ";
        }
        data << '\n';
    }

    std::cout << data.str() << std::endl;
}

void Simulation::print_log() {
    std::ostringstream data;

    data << "\nGeneration [" << (num_gen + 1) << "]:\n" << std::endl;

    for (int j = 0; j < (int)log_master[num_gen].size(); j++) {
        data << "[" << log_master[num_gen][j].x << ","
             << log_master[num_gen][j].y << "] ";
    }
    data << std::endl;

    std::cout << data.str() << std::endl;
}

void Simulation::print_petri() {
    std::ostringstream data;

    data << "\033[1;37m" << std::setfill('-') << std::setw(getNumCol() + 8)
         << "\033[0m" << std::endl;
    for (int i = 1; i < getNumRows() + 1; i++) {
        data << "\033[1;37m| \033[0m";
        for (int j = 1; j < getNumCol() + 1; j++) {
            if (petri_dish[i][j] == dead) {
                data << " ";
            } else {
                data << std::setw(1) << "\033[1;32m" << cell_char << "\033[0m";
            }
        }
        data << " \033[1;37m|\033[0m\n";
    }
    data << "\033[1;37m" << std::setfill('-') << std::setw(getNumCol() + 8)
         << "\033[0m" << std::endl;

    std::cout << data.str();
}

void Simulation::print_help() {
    std::cerr
        << "Usage: glife [<options>] <input_cfg_file>\n"
           "Simulation options:\n"
           "\t--help\t\t\tPrint this help text.\n"
           "\t--imgdir <path>\t\tSpecify directory where output images are "
           "written to.\n"
           "\t--maxgen <num>\t\tMaximum number of generations to simulate.\n"
           "\t--fps <num>\t\tNumber of generations presented per second.\n"
           "\t--blocksize <num>\tPixel size of a cell. Default = 5.\n"
           "\t--bkgcolor <color>\tColor name for the background. Default "
           "GREEN.\n"
           "\t--alivecolor <color>\tColor name for representing alive cells. "
           "Default "
           "RED.\n"
           "\t--outfile <filename>\tWrite the text representation of the "
           "simulation to "
           "the "
           "given filename.\n\n"
           "Available colors are:\n"
           "\tBLACK BLUE CRIMSON DARK_GREEN DEEP_SKY_BLUE DODGER_BLUE\n"
           "\tGREEN LIGHT_BLUE LIGHT_GREY LIGHT_YELLOW RED STEEL_BLUE\n"
           "\tWHITE YELLOW\n\n";
}

void Simulation::print_initial_msg() {
    std::cerr
        << "\n****************************************************************"
           "\n"
           "Welcome to Conway’s game of Life.\n"
           "Running a simulation on a grid of size "
        << getNumRows() << " by " << getNumCol()
        << " in which\n"
           "each cell can either be occupied by an organism or not.\n"
           "The occupied cells change from generation to generation\n"
           "according to the number of neighboring cells which are alive.\n"
           "****************************************************************\n";
}