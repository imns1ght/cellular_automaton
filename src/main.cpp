/*!
 * \file main.cpp
 * \author Jefferson Bruno, Ranieri Santos
 * \date 22/05/2019
 */

#include <../include/life.h>

/** ### Regras do game:
 *
 *1. Qualquer célula viva com menos de dois vizinhos vivos morre de solidão.\n
 *2. Qualquer célula viva com mais de três vizinhos vivos morre de superpopulação.\n
 *3. Qualquer célula morta com exatamente três vizinhos vivos se torna uma célula viva.\n
 *4. Qualquer célula viva com dois ou três vizinhos vivos continua no mesmo estado para a próxima geração.\n
 */
int main(int argc, char* argv[]) {
        Life game;

	// Show informations if success
	if (game.initialize(argc, argv) != -1) {
		// #TEST
		std::cout << game.getNumRows() << " " << game.getNumCol() << std::endl;
        	std::cout << game.getCellChar() << std::endl;
		game.print_board();
		std::cout << std::endl;
		game.print_matrix();    
        	std::cout << std::endl;
        	game.print_options();
        	std::cout << std::endl;
	}
	
        //=== OBJECTIVE ===
        /*
        life::lifeGame game;

        // Set up simulation
        game.initialize(argc, argv);

        // Initial message
        game.render();

        // Game Loop
        while (!game.game_over()) {
                game.process_events();
                game.update();
                game.render();
        }
        */

        return 0;
}