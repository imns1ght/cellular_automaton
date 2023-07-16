#ifndef SIM_H
#define Sim_H

// C
#include <getopt.h>  // getopt()
#include <unistd.h>  // usleep()

#include <cstdlib>  // atoi()

// C++
#include <fstream>    // std::ifstream
#include <iomanip>    // std::setw, std::setfill
#include <iostream>   // std::cout, std::cin
#include <limits>     // std::numeric_limits
#include <sstream>    // std::ostringstream
#include <stdexcept>  // std::invalid_argument
#include <string>     // std::string
#include <vector>     // std::vector

const int alive = 1;              //!< Alive cell.
const int dead = 0;               //!< Dead cell.
const int int_size = 2147483647;  //!< Standard value to maximum number of
                                  //!< generations, "Infinite".

class Simulation {
   private:
    //! Save the coordinates for each cell.
    struct Cell {
        int x, y;  //!< Coordinates of the cell.
    };

    /// Save command line arguments.
    struct Options {
        int maxgen = int_size;  //!< Maximum number of generations.
        int fps = 2;            //!< Number of generations presented per second.
        int blocksize = 10;     //!< Pixel size of a cell.
        std::string alivecolor =
            "RED";  //!< Color name for representing alive cells.
        std::string bkgcolor = "GREEN";  //!< Color name for the background.
        std::string imgdir =
            "data/images/";  //!< Directory where output images are written to.
        std::string inputfile;  //!< Filename for the input file.
        std::string outfile =
            "data/log.txt";  //!< Filename for the output file.
    } options;

    using log_struct =
        std::vector<Cell>;  //!< Store the coords for each generation.
    std::vector<log_struct>
        log_master;  //!< Store all generations from log_struct.
    std::vector<std::vector<int>> petri_dish;  //!< Where the cells lives...
    int num_rows, num_col;  //!< Dimensions of the petri_dish.
    char cell_char;         //!< Character that represent the cells.
    int num_gen = 0;        //!< Number of generations.

   public:
    //! Default constructor
    Simulation();

    //! Receive the dimensions of the petri_dish.
    Simulation(int nLin, int nCol);

    //! Desconstructor
    ~Simulation();

    /////////////////////////////////////////////
    // Client functions
    /////////////////////////////////////////////
    //! Start the simulation reading configurations from cli and file.
    void initialize(int argc, char *argv[]);

    /*!@brief Runs while extinction, stability, or maximum generation is not
     *reached.
     *@return true if extintion, stabilty or maximum generation has been
     *reached.
     */
    bool game_over();

    //! Apply the rules of conway's game of life, defining which cells will
    //! live.
    void process_events();

    //! Update the petri_dish with the new generation, adding and removing
    //! cells.
    void update();

    //! Process the output (text and images).
    void render();

   private:
    /*!@brief Verify if the current generation is extinct.
     *@return true if not cell was stored in the log.
     */
    bool extinct();

    /*!@brief Verify if the current generation is equal to the previous
     *generations.
     *@return true if the current generation is equal to a previous generation.
     */
    bool stable();

   private:
    /////////////////////////////////////////////
    // Get virtual values of the petri_dish
    /////////////////////////////////////////////
    //! Get the number of rows.
    inline int getNumRows() { return num_rows - 2; }

    //! Get the number of columns.
    inline int getNumCol() { return num_col - 2; }

    //! Get the char that represent an alive cell.
    inline char getCellChar() { return cell_char; }

    /////////////////////////////////////////////
    // Manipulation functions
    /////////////////////////////////////////////
    //! Prepare the petri_dish to store cells.
    void prepare_petri(int size_row, int size_col);

    /*!@brief Check how many neighbors a cell has.
     *@param Coordinate x of the cell.
     *@param Coordinate y of the cell.
     *@return Number of neighbors.
     */
    int surroundings(int i, int j);

    //! Define living cells applying the rules.
    void set_alive();

    /////////////////////////////////////////////
    // I/O functions
    /////////////////////////////////////////////

    /*!@brief Read the arguments from cli.
     * @param Number of arguments.
     * @param Array of arguments.
     * @return 0 if success, and -1 if failed.
     */
    int read_options(int argc, char *argv[]);

    /*!@brief Read the configuration file.
     */
    void read_file();

    /////////////////////////////////////////////
    // Print functions
    /////////////////////////////////////////////

    //! Show the command line arguments.
    void print_options();

    //! Show the matrix.
    void print_matrix();

    //! Show the log.
    void print_log();

    //! Show the petri_dish.
    void print_petri();

    //! Show the arguments available from cli.
    void print_help();

    //! Show the initial message.
    void print_initial_msg();
};

#endif