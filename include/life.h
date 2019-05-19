#ifndef LIFE_H
#define LIFE_H

#include <getopt.h>   // getopt()
#include <cstdlib>    // atoi()
#include <cstring>    // memset()
#include <fstream>    // std::ifstream
#include <iomanip>    // setw, setfill
#include <iostream>   // std::cout, std::cin
#include <sstream>    // ostringstream
#include <stdexcept>  // std::invalid_argument
#include <string>     // std::string
#include <vector>     // std::vector

const int empty = -1;  //!< No cell
const int dead = 0;    //!< Dead cell
const int alive = 1;   //!< Alive cell

/// Status of the cell
class Cell {
        int x, y;    //!< Coordinates
        bool alive;  //!< Cell state
};

/// Individual life
class Life {
        using data_type = int;                      //!< The value type.
        using pointer = data_type *;                //!< Pointer to a value stored in the container.
        using reference = data_type &;              //!< Reference to a value stored in the container.
        using const_reference = const data_type &;  //!< Const reference to a value stored in the container.

       private:
        data_type num_rows, num_col;                // Dimensions of the matrix.
        char cell_char;                             // Represent the cells.
        std::vector<std::vector<data_type>> board;  // Simulation board.

       public:
        /// Default constructor
        Life() {}

        /// Desconstructor
        ~Life() {}

        /// Set the number of rows
        void setNumRows(int n) { num_rows = n; }

        /// Get the number of rows
        int getNumRows() { return num_rows; }

        /// Set the number of columns.
        void setNumCol(int n) { num_col = n; }

        /// Get the number of columns
        int getNumCol() { return num_col; }

        /// Set the char that represent an alive cell
        void setCellChar(char a) { cell_char = a; }

        /// Get the char that represent an alive cell
        char getCellChar() { return cell_char; }

       private:
        void prepare_matrix(data_type size_row, data_type size_col) {
                num_rows = size_row;
                num_col = size_col;

                board.resize(num_rows);  // Define the number of lines

                // In every row, create n columns with -1 (indicate empty space),
                // thus this is equivalent to a num_rows x num_col.
                for (int i = 0; i < num_rows; i++) {
                        board[i].assign(num_col, empty);
                }
        }

        /// Save command line arguments.
        struct Options {
                int maxgen = 100;                      //!< Maximum number of generations.
                int fps = 2;                           //!< Number of generations presented per second.
                int blocksize = 5;                     //!< Pixel size of a cell.
                std::string alivecolor = "RED";        //!< Color name for representing alive cells.
                std::string bkgcolor = "GREEN";        //!< Color name for the background.
                std::string imgdir = "data/images/";   //!< Directory where output images are written to.
                std::string inputfile;                 //!< Filename for the input file.
                std::string outfile = "data/log.txt";  //!< Filename for the output file.

        } options;  //!< Initialize the struct.

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
                                                show_help();
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
        int read_file() {
                std::ifstream file;
                file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

                try {
                        file.open(options.inputfile);  //!< Open the file in reading mode.

                        file >> num_rows >> num_col;
                        file >> cell_char;

                        prepare_matrix(num_rows, num_col);  // Prepare the matrix to store the cells.

                        int i = 0;
                        std::string line;
                        while ((std::getline(file, line)) && (i < getNumRows())) {
                                for (auto j = 0; j < (int)line.size() && j < getNumCol(); j++) {
                                        if (line[j] == getCellChar()) {
                                                board[i][j] = 1;  // Alive cell
                                        }
                                }

                                i++;
                        }

                        file.close();

                } catch (std::ifstream::failure &e) {
                        return -1;
                }

                return 0;
        }

       public:
        /// Print the command line arguments.
        void print_options() {
                std::cout << ">>> Options <<< \n";
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

                for (int i = 0; i < num_rows; i++) {
                        for (int j = 0; j < num_col; j++) {
                                data << std::setw(2) << board[i][j] << " ";
                        }
                        data << '\n';
                }

                std::cout << data.str();
        }

        /// Print the board.
        void print_board() {
                std::ostringstream data;

                for (int i = 0; i < num_rows; i++) {
                        data << "[ ";
                        for (int j = 0; j < num_col; j++) {
                                if (board[i][j] == -1) {
                                        data << " ";
                                } else {
                                        data << std::setw(1) << cell_char << "";
                                }
                        }
                        data << " ]\n";
                }

                std::cout << data.str();
        }

        /// Show the arguments available from cli.
        void show_help() {
                std::cerr
                    << "Usage: glife [<options>] <input_cfg_file>\n"
                       "Simulation options:\n"
                       "\t--help\t\t\tPrint this help text.\n"
                       "\t--imgdir <path>\t\tSpecify directory where output images are written to.\n"
                       "\t--maxgen <num>\t\tMaximum number of generations to simulate.\n"
                       "\t--fps <num>\t\tNumber of generations presented per second.\n"
                       "\t--blocksize <num>\tPixel size of a cell. Default = 5.\n"
                       "\t--bkgcolor <color>\tColor name for the background. Default GREEN.\n"
                       "\t--alivecolor <color>\tColor name for representing alive cells. Default RED.\n"
                       "\t--outfile <filename>\tWrite the text representation of the simulation to the given "
                       "filename.\n\n"
                       "Available colors are:\n"
                       "\tBLACK BLUE CRIMSON DARK_GREEN DEEP_SKY_BLUE DODGER_BLUE\n"
                       "\tGREEN LIGHT_BLUE LIGHT_GREY LIGHT_YELLOW RED STEEL_BLUE\n"
                       "\tWHITE YELLOW\n\n";
        }

        int initialize(int argc, char *argv[]) {
                try {
                        read_options(argc, argv);
                } catch (const std::invalid_argument &ia) {
                        std::cerr << "\033[0;31m>>> Error: " << ia.what() << "\033[0m\n";
                        show_help();
                        return -1;
                }

                if (read_file() == -1) {
                        std::cerr << "\033[0;31m>>> Error: Exception opening/reading/closing file\033[0m\n";
                        return -1;
                }

                return 0;
        }
};

#endif