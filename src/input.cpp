#include "../include/simulation.h"

int Simulation::read_options(int argc, char *argv[]) {
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
                std::cerr << "\n\033[0;31mMissing argument.\033[0m\n\n";
                print_help();
                return -1;
        }

        int opt;
        while (optind < argc) {
                if ((opt = getopt_long(argc, argv, "hd:m:f:s:b:a:o:", tmp, NULL)) != -1) {
                        switch (opt) {
                                case 'h': /* -h or --help */
                                        print_help();
                                        return -1;
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
                                        print_help();
                                        return -1;
                        }
                } else {
                        options.inputfile = argv[optind++];  // Path to data file
                }
        }

        // Verify if the path to data file was entered correctly.
        if (options.inputfile == "") {
                std::cerr
                    << "\n\033[0;31mMissing data file or the path was entered before the options.\033[0m\n\n";
                print_help();
                return -1;
        }

        return 0;
}

void Simulation::read_file() {
        std::ifstream file;
        file.exceptions(std::ifstream::badbit);

        std::cout << ">>> Trying to open input file [" << options.inputfile << "]... ";
        file.open(options.inputfile);  // Open the file in reading mode.

        // Verify if successfully opened the file.
        if (!file) {
                std::cerr << "\n\033[0;31m>>> Error: opening/reading file\033[0m\n";
                exit(EXIT_FAILURE);
        }

        std::cout << "done!\n";
        std::cout << ">>> Processing data, please wait...\n";

        file >> num_rows >> num_col;  // Store the dimensions of the petri_dish
        file >> cell_char;            // Store the char who will represent a living cell.

        std::cout << ">>> Grid size: " << num_rows << " rows by " << num_col << " cols.\n";
        std::cout << ">>> Character that represents a living cell: '" << cell_char << "'\n";

        // Prepare the matrix where the cells are stored.
        prepare_petri(num_rows, num_col);

        int i = 0;
        std::string line;
        // Save just the living cells represented by 'cell_char'.
        while ((std::getline(file, line)) && (i < getNumRows() + 2)) {
                for (auto j = 0; j < (int)line.size() && (j < getNumCol() + 2); j++) {
                        if (line[j] == getCellChar()) {
                                petri_dish[i][j + 1] = alive;  // Alive cell
                        }
                }

                i++;
        }

        file.close();

        std::cout << ">>> Finished reading input data file." << std::endl;
}
