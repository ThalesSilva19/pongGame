#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>
#include <iostream>

#include"pong.cpp"

int sock_client;
struct sockaddr_in addr;

//A thread do jogo onde ele calcula todas posições e manda para o cliente.
void game_thread(GameInfo* game){
    while(game->score_b < 10 && game->score_a < 10){
		game->action();
		game->print_screen();
		usleep(200000);
        char message[8];
        message[0] = game->score_a;
        message[1] = game->score_b;
        message[2] = game->ball_x;
        message[3] = game->ball_y;
        message[4] = game->speed_x;
        message[5] = game->speed_y;
        message[6] = game->player_a;
        message[7] = game->player_b;
        send(sock_client, message, 8, 0);
	}
}

//A thread que escuta se o jogador b se movimentou.
void reciver_thread(GameInfo* game){
    char response[1];
    while(game->score_b < 10 && game->score_a < 10){
        recv(sock_client, response, 1, 0); /* Recebe mensagem do cliente */
        if(response[0] == 1){
            game->player_b += 3;
            if(game->player_b > FIELD_WIDTH - PLAYER_WIDTH){
                game->player_b = FIELD_WIDTH - PLAYER_WIDTH;
            }
        }
        else if(response[0] == 0){
            game->player_b -=3;
            if(game->player_b < 0) {
                game->player_b = 0;
            }
        }
    }
}

//A thread que lê o teclado para movimentar o jogador a.
void keyEvents(GameInfo* game){	
	while(game->score_b < 10 && game->score_a < 10){
		char code = getchar();
		if(code == 	'd'){
			game->player_a+= 3;
			if(game->player_a > FIELD_WIDTH - PLAYER_WIDTH){
				game->player_a = FIELD_WIDTH - PLAYER_WIDTH;
			}
		}
		else if(code == 'a'){
			game->player_a -=3;
			if(game->player_a < 0) {
				game->player_a = 0;
			}
		}
		char message[8];
		message[0] = game->score_a;
		message[1] = game->score_b;
		message[2] = game->ball_x;
		message[3] = game->ball_y;
		message[4] = game->speed_x;
		message[5] = game->speed_y;
		message[6] = game->player_a;
		message[7] = game->player_b;
		send(sock_client, message, 8, 0);
	}
}


//Inicializa o servidor e começa o loop do jogo.
int main(void) {
    int meu_socket;
    meu_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (meu_socket == -1){
        printf("Erro ao criar o socket!\n");
        return 1;
    }

    addr.sin_family = AF_INET;
   	addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = INADDR_ANY;
    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

    if (bind(meu_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1){
        printf("Erro na funcao bind()\n");
        return 1;
    }

    if (listen(meu_socket, 1) == -1){
        printf("Erro na funcao listen()\n");
        return 1;
    }

    printf("Aguardando oponente...\n");

    sock_client = accept(meu_socket, 0, 0);

    if (sock_client == -1){
        printf("\nErro na funcao accept()\n");
        return 1;
    }

    GameInfo game;

    thread key (keyEvents, &game);
    thread reciever (reciver_thread, &game);
    thread sender (game_thread, &game);
    game.kickout(1);
    key.join();
    reciever.join();
    sender.join();

    return 1;
}
