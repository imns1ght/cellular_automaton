/*!
 * \file main.cpp
 * \author Jefferson Bruno, Ranieri Santos
 * \date 22/05/2019
 */

#include <../include/simulation.h>

int main(int argc, char* argv[]) {
        Simulation game;

	// Set up simulation.
        game.initialize(argc, argv);

        // Initial message.
        game.render();

        // Game Loop
        while (!game.game_over()) {
                game.process_events(); // Verificar REGRAS.
                game.update();         // Atualizar o board.
                game.render();         // Exibir o board
                usleep(1000000);
        }

        return 0;
}