/*!
 * \file main.cpp
 * \author Jefferson Bruno, Ranieri Santos
 * \date 22/05/2019
 */

#include <../include/life.h>

/** ### Regras do game:
 *
 * 1. Qualquer célula viva com menos de dois vizinhos vivos morre de solidão.\n
 * 2. Qualquer célula viva com mais de três vizinhos vivos morre de superpopulação.\n
 * 3. Qualquer célula morta com exatamente três vizinhos vivos se torna uma célula viva.\n
 * 4. Qualquer célula viva com dois ou três vizinhos vivos continua no mesmo estado para a próxima geração.\n
 *
 * A simulacão deve ser finalizada em três situações diferentes: 
 * 1. o número máximo de gerações foi alcançado (se esse valor for especificado);\n
 * 2. a configuração da geração atual é considerada estável (não muda em relação a anterior);\n
 * 3. a configuração da geração atual é extinta (todas celulas mortas).\n 
 */
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
                usleep(microseconds);
        }

        return 0;
}