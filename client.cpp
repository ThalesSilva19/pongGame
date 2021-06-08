#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <iostream>

#include "pong.cpp"

int meu_socket;
struct sockaddr_in addr;

//Imprime a tela do jogo em intervalos regualres de tempo.
void game_thread(GameInfo* game){
    while(game->score_b < 10 && game->score_a < 10){
		game->print_screen();
		usleep(100000);
	}
}

//Recebe as informações do jogo vindo do servidor.
void reciver_thread(GameInfo* game){
    char response[8];
    while (game->score_b < 10 && game->score_a < 10){
        recv(meu_socket, response, 8, 0);
        game->score_a = response[0];
        game->score_b = response[1];
        game->ball_x = response[2];
        game->ball_y = response[3];
        game->speed_x = response[4];
        game->speed_y = response[5];
        game->player_a = response[6];
        game->player_b = response[7];
    }
}

//Lê o teclado e manda os comandos para o servidor.
void keyEvents(GameInfo* game){	
	while(game->score_b < 10 && game->score_a < 10){	
		char code = getchar();
		printf("%d\n",code);
		char message[1];
		message[0] = -1;
		if(code == 'a'){
			message[0] = 0;
			send(meu_socket, message, 1, 0);
		}
		else if(code == 'd'){
			message[0] = 1;
			send(meu_socket, message, 1, 0);
		}
	}
}

//Tenta conectar no servidor e inicia o loop do jogo.
int main()
{

    meu_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (meu_socket == -1)
    {
        printf("Erro ao criar o socket!\n");
        return 1;
    }


	//Ip e Porta hardcoded do servidor
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2201);
    addr.sin_addr.s_addr = inet_addr("34.202.4.119");

    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

    printf("Tentando se conectar ao servidor...\n");

    if (connect(meu_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        printf("Erro ao se conectar!\n");
        return 1;
    }

    printf("Conectado!\n\n");

    GameInfo game;

    thread key (keyEvents, &game);
    thread reciever (reciver_thread, &game);
    thread sender (game_thread, &game);
    game.kickout(1);
    key.join();
    reciever.join();
    sender.join();
}

