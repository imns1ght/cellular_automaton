#ifndef LIFE_H
#define LIFE_H

#include <getopt.h>   // getopt()
#include <cstdlib>    // atoi()
#include <fstream>    // std::ifstream
#include <iomanip>    // setw, setfill
#include <iostream>   // std::cout, std::cin
#include <limits>     // std::numeric_limits
#include <sstream>    // std::ostringstream
#include <stdexcept>  // std::invalid_argument
#include <string>     // std::string
#include <vector>     // std::vector

const int alive = 1;  //!< Alive cell
const int dead = 0;   //!< Dead cell

/// Individual life
class Simulation {
        using data_type = int;                      //!< The value type.
        using pointer = data_type *;                //!< Pointer to a value stored in the container.
        using reference = data_type &;              //!< Reference to a value stored in the container.
        using const_reference = const data_type &;  //!< Const reference to a value stored in the container.

       private:
        struct Cell;
        data_type num_rows, num_col;                // Dimensions of the matrix.
        char cell_char;                             // Represent the cells.
        std::vector<std::vector<int>> board;        // Simulation board.
        std::vector<std::vector<Cell>> log_matrix;  // Simulation board.

        /// Cell
        struct Cell {
                int x, y;  // Coordenates
        };

        /// Save command line arguments.
        struct Options {
                int maxgen = 3;                        //!< Maximum number of generations.
                int fps = 2;                           //!< Number of generations presented per second.
                int blocksize = 5;                     //!< Pixel size of a cell.
                std::string alivecolor = "RED";        //!< Color name for representing alive cells.
                std::string bkgcolor = "GREEN";        //!< Color name for the background.
                std::string imgdir = "data/images/";   //!< Directory where output images are written to.
                std::string inputfile;                 //!< Filename for the input file.
                std::string outfile = "data/log.txt";  //!< Filename for the output file.

        } options;  //!< Initialize the struct.

       public:
        /// Default constructor
        Simulation() {}

        /// Constructor
        Simulation(int nLin, int nCol) { prepare_board(nLin, nCol); }

        /// Desconstructor
        ~Simulation() {}

        /// Set the number of rows
        void setNumRows(int n) { num_rows = n; }

        /// Get the number of rows
        int getNumRows() { return num_rows - 2; }

        /// Set the number of columns.
        void setNumCol(int n) { num_col = n; }

        /// Get the number of columns
        int getNumCol() { return num_col - 2; }

        /// Set the char that represent an alive cell
        void setCellChar(char a) { cell_char = a; }

        /// Get the char that represent an alive cell
        char getCellChar() { return cell_char; }

       private:
        void prepare_board(int size_row, int size_col) {
                num_rows = size_row + 2;  // Two more rows to prevent error.
                num_col = size_col + 2;   // Two more col to prevent error.

                board.resize(num_rows);  // Define the number of lines

                // In every row, create n columns with -1 (indicate empty space),
                // thus this is equivalent to a num_rows x num_col.
                for (int i = 0; i < num_rows; i++) {
                        board[i].assign(num_col, dead);
                }
        }

        void prepare_log() {
                log_matrix.resize(options.maxgen);  // Define the number of lines

                // In every row, create n columns with -1 (indicate empty space),
                // thus this is equivalent to a num_rows x num_col.
                for (int i = 0; i < options.maxgen; i++) {
                        log_matrix[i].resize((getNumRows() * getNumCol()) / 3);
                }
        }

        /// Read the arguments from cli.
        /*!
         * \param argc Number of arguments.
         * \param argv Array of arguments.
         * \param &life Object used to save options
         * \throw std::invalid_argument
         * \return 0 if success, and -1 if failed.
         */
        int read_options(int argc, char *argv[]) {
                // Valid options available to read.
                const struct option tmp[9] = {
                    {"help", no_argument, 0, 'h'},
                    {"imgdir", 1, 0, 'd'},
                    {"maxgen", 1, 0, 'm'},
                    {"fps", 1, 0, 'f'},
                    {"blocksize", 1, 0, 's'},
                    {"bkgcolor", 1, 0, 'b'},
                    {"alivecolor", 1, 0, 'a'},
                    {"outfile", 1, 0, 'o'},
                    {0, 0, 0, 0},
                };

                // If there's no argument.
                if (argc < 2) {
                        throw std::invalid_argument("Missing argument!\n");
                        return -1;
                }

                int opt;
                while (optind < argc) {
                        if ((opt = getopt_long(argc, argv, "hd:m:f:s:b:a:o:", tmp, NULL)) != -1) {
                                switch (opt) {
                                        case 'h': /* -h or --help */
                                                print_help();
                                                return 0;
                                        case 'd': /* -d or --imgdir */
                                                options.imgdir = optarg;
                                                break;
                                        case 'm': /* -m or --maxgen */
                                                options.maxgen = atoi(optarg);
                                                break;
                                        case 'f': /* -f or --fps */
                                                options.fps = atoi(optarg);
                                                break;
                                        case 's': /* -s or --blocksize */
                                                options.blocksize = atoi(optarg);
                                                break;
                                        case 'b': /* -b or --bkgcolor */
                                                options.bkgcolor = optarg;
                                                break;
                                        case 'a': /* -a or --alivecolors */
                                                options.alivecolor = optarg;
                                                break;
                                        case 'o': /* -s or --outfile */
                                                options.outfile = optarg;
                                                break;

                                        // No valid arguments provided.
                                        default:
                                                throw std::invalid_argument("Invalid argument!\n");
                                                return -1;
                                }
                        } else {
                                options.inputfile = argv[optind++];  // Path to data file
                        }
                }

                // Verify if the path to data file was entered correctly.
                if (options.inputfile == "") {
                        throw std::invalid_argument(
                            "Missing data file or the path was entered before the options.\n");
                        return -1;
                }

                return 0;
        }

        /// Read config from data file.
        /*!
         * \return 0 if success, and -1 if failed.
         */
        void read_file() {
                std::ifstream file;
                file.exceptions(std::ifstream::badbit);

                try {
                        std::cout << ">>> Trying to open input file [" << options.inputfile << "]... ";
                        file.open(options.inputfile);  //!< Open the file in reading mode.

                        if (!file) {
                                throw std::ifstream::failure("");
                        }

                } catch (std::ifstream::failure &e) {
                        std::cerr << "\n\033[0;31m>>> Error: opening/reading file\033[0m\n";
                        exit(EXIT_FAILURE);
                }

                std::cout << "done!\n";
                std::cout << ">>> Processing data, please wait...\n";

                file >> num_rows >> num_col;
                file >> cell_char;

                std::cout << ">>> Grid size: " << num_rows << " rows by " << num_col << " cols.\n";
                std::cout << ">>> Character that represents a living cell: '" << cell_char << "'\n";

                prepare_board(num_rows, num_col);  // Prepare the matrix to store the cells.

                int i = 0;
                std::string line;
                while ((std::getline(file, line)) && (i < getNumRows() + 2)) {
                        for (auto j = 0; j < (int)line.size() && (j < getNumCol() + 2); j++) {
                                if (line[j] == getCellChar()) {
                                        board[i][j + 1] = alive;  // Alive cell
                                }
                        }

                        i++;
                }

                file.close();

                std::cout << ">>> Finished reading input data file." << std::endl;
        }

        int surroundings(int i, int j) {
                int n = 0;  // Number of neighbors

                if ((i > getNumRows()) || (j > getNumCol())) {
                        std::cerr << "\n\033[0;31m>>> Error: out of range!\033[0m\n";
                        exit(EXIT_FAILURE);
                }

                if (board[i - 1][j - 1] == alive) {
                        n++;
                }

                if (board[i - 1][j] == alive) {
                        n++;
                }

                if (board[i - 1][j + 1] == alive) {
                        n++;
                }

                if (board[i][j - 1] == alive) {
                        n++;
                }

                if (board[i][j + 1] == alive) {
                        n++;
                }

                if (board[i + 1][j - 1] == alive) {
                        n++;
                }

                if (board[i + 1][j] == alive) {
                        n++;
                }

                if (board[i + 1][j + 1] == alive) {
                        n++;
                }

                return n;
        }

        //! Store living cells in log_matrix.
        int setAlive(int num_generation) {
                int m_cell = 0;  // Position of the cell (struct)

                // Copy living cells from board to log of generation.
                for (int i = 0; i <= getNumRows(); i++) {
                        for (int j = 0; j <= getNumCol(); j++) {
                                if (board[i][j] == alive) {
                                        log_matrix[num_generation][m_cell].x = i;
                                        log_matrix[num_generation][m_cell].y = j;
                                        m_cell++;
                                }
                        }
                }

                return ++num_generation;  // Number of generations
        }

       public:
        void initialize(int argc, char *argv[]) {
                try {
                        // Read arguments from cli.
                        read_options(argc, argv);
                } catch (const std::invalid_argument &ia) {
                        std::cerr << "\033[0;31m>>> Error: " << ia.what() << "\033[0m\n";
                        print_help();
                        exit(EXIT_FAILURE);
                }

                // Read the config file.
                read_file();

                // Prepare the matrix to store the coordenates of living cells.
                prepare_log();
        }

        int game_over() {
                // #TODO

                // extinct()
                // stable()

                return 0;
        }

        void process_events() {
                // #TODO
        }

        void update() {
                // #TODO
        }

        void render() {
                // #TODO
        }

        /// Print the command line arguments.
        void print_options() {
                std::cout << "[Options]\n\n";
                std::cout << "imgdir: \"" << options.imgdir << "\"" << std::endl;
                std::cout << "maxgen: " << options.maxgen << std::endl;
                std::cout << "fps: " << options.fps << std::endl;
                std::cout << "blocksize: " << options.blocksize << std::endl;
                std::cout << "bkgcolor: " << options.bkgcolor << std::endl;
                std::cout << "alivecolor: " << options.alivecolor << std::endl;
                std::cout << "inputfile: \"" << options.inputfile << "\"" << std::endl;
                std::cout << "outfile: \"" << options.outfile << "\"" << std::endl;
        }

        /// Print the matrix.
        void print_matrix() {
                std::ostringstream data;

                std::cout << "[" << num_rows << " x " << num_col << "]\n" << std::endl;
                for (int i = 0; i < getNumRows() + 2; i++) {
                        if (i < 10) {
                                data << "[" << i << "]" << std::setw(2) << " ";
                        } else {
                                data << "[" << i << "]" << std::setw(1) << " ";
                        }

                        for (int j = 0; j < getNumCol() + 2; j++) {
                                data << std::setw(1) << board[i][j] << " ";
                        }
                        data << '\n';
                }

                std::cout << data.str() << std::endl;
        }

        void print_log() {
                std::ostringstream data;

                for (int i = 0; i < options.maxgen; i++) {
                        data << "\nGeneration: [" << i << "]\n" << std::endl;

                        for (int j = 0; j < log_matrix[i].size(); j++) {
                                // if ((log_matrix[i][j].x != 0) && (log_matrix[i][j].y != 0)) {
                                data << log_matrix[i][j].x << " " << log_matrix[i][j].y << " \n";
                                //}
                        }
                        data << '\n';
                }

                std::cout << data.str() << std::endl;
        }

        /// Print the board.
        void print_board() {
                std::ostringstream data;

                data << "\033[1;37m" << std::setfill('-') << std::setw(getNumCol() + 8) << "\033[0m"
                     << std::endl;
                for (int i = 1; i < getNumRows() + 1; i++) {
                        data << "\033[1;37m| \033[0m";
                        for (int j = 1; j < getNumCol() + 1; j++) {
                                if (board[i][j] == dead) {
                                        data << " ";
                                } else {
                                        data << std::setw(1) << "\033[1;32m" << cell_char << "\033[0m";
                                }
                        }
                        data << " \033[1;37m|\033[0m\n";
                }
                data << "\033[1;37m" << std::setfill('-') << std::setw(getNumCol() + 8) << "\033[0m"
                     << std::endl;

                std::cout << data.str();
        }

        /// Show the arguments available from cli.
        void print_help() {
                std::cerr << "Usage: glife [<options>] <input_cfg_file>\n"
                             "Simulation options:\n"
                             "\t--help\t\t\tPrint this help text.\n"
                             "\t--imgdir <path>\t\tSpecify directory where output images are written to.\n"
                             "\t--maxgen <num>\t\tMaximum number of generations to simulate.\n"
                             "\t--fps <num>\t\tNumber of generations presented per second.\n"
                             "\t--blocksize <num>\tPixel size of a cell. Default = 5.\n"
                             "\t--bkgcolor <color>\tColor name for the background. Default GREEN.\n"
                             "\t--alivecolor <color>\tColor name for representing alive cells. Default RED.\n"
                             "\t--outfile <filename>\tWrite the text representation of the simulation to the "
                             "given filename.\n\n"
                             "Available colors are:\n"
                             "\tBLACK BLUE CRIMSON DARK_GREEN DEEP_SKY_BLUE DODGER_BLUE\n"
                             "\tGREEN LIGHT_BLUE LIGHT_GREY LIGHT_YELLOW RED STEEL_BLUE\n"
                             "\tWHITE YELLOW\n\n";
        }

        // Initial message
        void print_initial_msg() {
                std::cerr << "\n****************************************************************\n"
                             "Welcome to Conway’s game of Life.\n"
                             "Running a simulation on a grid of size "
                          << getNumRows() << " by " << getNumCol()
                          << " in which\n"
                             "each cell can either be occupied by an organism or not.\n"
                             "The occupied cells change from generation to generation\n"
                             "according to the number of neighboring cells which are alive.\n"
                             "****************************************************************\n";
        }
};

#endif