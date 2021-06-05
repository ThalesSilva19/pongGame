#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<iostream>
#include<fstream>
#include<thread>
#include<curses.h>

using namespace std;

#define FIELD_WIDTH 51
#define FIELD_HEIGHT 31
#define PLAYER_WIDTH 10

class GameInfo{
	public:
	int ball_x = 5;
	int ball_y = 1;
	int speed_x = 1;
	int speed_y = 1;
	int player_a = 6;
	int player_b = 6;
	int score_a = 0;
	int score_b = 0;

	void kickout(int side){
		speed_y = side;
		ball_y = FIELD_HEIGHT/2;
		ball_x = (rand() % (FIELD_WIDTH - 2)) + 1;
		if(rand() % 2 == 1) {
			speed_x = 1;
		}
		else {
			speed_x = -1;
		}
	}

	void action(){
		ball_x += speed_x;
		ball_y += speed_y;
		if(ball_x == 0 || ball_x == FIELD_WIDTH-1){
			speed_x = -speed_x;
		}
		if(ball_y == 1 && (ball_x > player_a && ball_x < player_a + PLAYER_WIDTH)){
			speed_y = 1;
		}
		if(ball_y == FIELD_HEIGHT-1 && (ball_x > player_b && ball_x < player_b + PLAYER_WIDTH)){
			speed_y = -1;
		}
		if(ball_y == -1){
			score_b++;
			kickout(1);
		}
		if(ball_y == FIELD_HEIGHT+1){
			score_a++;
			kickout(-1);
		}
	}

	void print_line(int num){
		if(ball_y == num){
			printf("|");
			for(int i = 0; i < ball_x;i++){
				printf(" ");
			}
			printf("@");
			for(int i = ball_x+1; i < FIELD_WIDTH;i++){
				printf(" ");
			}
			printf("|\n");
		}
		else{
			printf("|");
			for(int i = 0; i < FIELD_WIDTH;i++){
				printf(" ");
			}
			printf("|\n");
		}
	}

	void print_player_a(){
		printf("|");
		for(int i = 0; i < FIELD_WIDTH;i++){
			if(i > player_a && i < player_a + PLAYER_WIDTH){
				printf("-");
			}
			else if(ball_y == 0 && ball_x == i){
				printf("@");
			}
			else{
				printf(" ");
			}
		}
		printf("|\n");
	}

	void print_player_b(){
		printf("|");
		for(int i = 0; i < FIELD_WIDTH;i++){
			if(i > player_b && i < player_b + PLAYER_WIDTH){
				printf("-");
			}
			else if(ball_y == FIELD_HEIGHT && ball_x == i){
				printf("@");
			}
			else{
				printf(" ");
			}
		}
		printf("|\n");
	}


	void print_score(){
		for(int i = 0; i < (FIELD_WIDTH-5)/2;i++){
			printf(" ");
		}
		printf("%d x %d",score_a,score_b);
		for(int i = (FIELD_WIDTH+5)/2; i < FIELD_WIDTH - 5;i++){
			printf(" ");
		}
		printf("\n");
	}

	void print_screen(){
		printf("\033c\n");

		print_score();
		print_player_b();
		for(int i = FIELD_HEIGHT -1; i > 0; i--){
			print_line(i);
		}
		print_player_a();
	}
};
