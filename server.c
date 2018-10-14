#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define NUM_TILES_X 9
#define NUM_TILES_Y 9
#define NUM_MINES 10
#define MAXDATASIZE 1000

#define BACKLOG 10

int number_user=0;


int tester[9][9];

struct user{
	char name[20];
	char password[6];

};

typedef struct user auth_users;

#define NUM_TILES_X 9
#define NUM_TILES_Y 9
#define NUM_MINES 10
#define MAXDATASIZE 1000 /* max number of bytes we can get at once */

typedef struct
{
	int adjacent_mines;
	bool revealed;
	bool is_mine;
} Tile;

typedef struct 
{
	int great;
	Tile tiles[NUM_TILES_X][NUM_TILES_Y];
}Gamestate;




bool tile_contains_mine(int x, int y, Gamestate *currentstate){
	if(currentstate->tiles[x][y].is_mine == true){
		return true;
	} else{
		return false;
	}


}

void place_mines(Gamestate *currentstate)
{
	/*resetting gamestate*/
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			currentstate->tiles[i][j].is_mine = false;
			currentstate->tiles[i][j].revealed = false;
			currentstate->tiles[i][j].adjacent_mines = 0;
		}
	}
	
	for(int i = 0; i < NUM_MINES; i++)
	{
		int x, y;
		do{
			x = rand() % NUM_TILES_X;
			y = rand() % NUM_TILES_Y;
			
			
		} while(tile_contains_mine(x, y, currentstate));
		printf(" %d,%d ",x,y);
		currentstate->tiles[x][y].is_mine = true;
		
	
		
	}
}

int *Receive_Array_Int_data(int socket_identifier, int size){
	int number_of_bytes, i=0;
	uint16_t statistics;
	int *results = malloc(sizeof(int)*size);
	
	for(i=0;i<size;i++){
	
		if((number_of_bytes = recv(socket_identifier, &statistics, sizeof(uint16_t),0))== -1){
			perror("recv");
			exit(EXIT_FAILURE);
		}
		
		results[i] = ntohs(statistics);
	}
	
	return results;
	
}


char *Recieve_message(int socket){
	char *buf = malloc(sizeof(char)*MAXDATASIZE);
	int numbytes;
	
	if((numbytes = recv(socket, buf, MAXDATASIZE,0))==-1){
		perror("recv");
		exit(1);
	}
	
	buf[numbytes] = '\0';
	buf[numbytes] = '\0';
	return buf;

}

void Send_message(int socket, const void* message){
	if(send(socket, message,1000,0)==-1){
				perror("send");
			}

}

int main(int argc, char **argv){
	
	char *name;
	char *password;





	FILE *input_file = fopen("Authentication.txt","r");
	int line = 0;

	char input[512];
	int letter;

	if(input_file == NULL){
		perror("Error opening file");
		return(-1);

	} do {
		letter = fgetc(input_file);
		if(feof(input_file)){
			break;
		}
		/*do stuff here */
		printf("%c", letter);
	} while(1);
	/*
	rewind(input_file);

	while(fgets(input,512,input_file)){
		line++;
		printf("| %d -> %s |",line, input);
		
		

	}
	*/

	auth_users *reguser = malloc(sizeof(auth_users)*20);
	reguser[0].name[0] = 'w';

	

	/*
	char str[60];
	
	if(input_file == 0){
		perror("Can't open the file\n");
		exit(-1);
	} else {
		int character = 0;
		int line_index = 0;
		
		while(!feof(input_file)){
			character = fgetc(input_file);
			if(character == '\n'){
				line_index++;
			}
		}
		
		number_user = line_index - 1;
		rewind(input_file);
		*/
		/*stuff in function
		char str[60];
		int index = 0;
		while(!feof(input_file)){
			character = fgetc(input_file);
			if(character == '\n'){
				while(index < line_index -1){
					fgets(str, 60, input_file);
					int index_username = 0;
					int index_password = 0;
					for(int j = 0; j < strlen(str); j++){
					}
				}
			}
		}
		
	}
	*/
	
	fclose(input_file);










	int sockfd, new_fd; /*listen on sock_fd, new connection on new_fd */
	struct sockaddr_in my_addr; /* mu address information */
	struct sockaddr_in their_addr; /* connector's address information */
	socklen_t sin_size;
	int i=0;
	int socket_number;
	
	/* Get port number for server to listen on */
	if(argc != 2){
		socket_number = 12345;
	} else {
		socket_number = atoi(argv[1]);
	}
	
	/* generate the socket */
	if((sockfd = socket(AF_INET, SOCK_STREAM,0)) == -1) {
		perror("socket");
		exit(1);
	}
	
	/*generate the socket */
	my_addr.sin_family = AF_INET; /*host byte order (internet domain)*/
	my_addr.sin_port = htons(socket_number); /* short, network byte order htons converts also assigning port number*/
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto fill with my IP address */
	
	/* Bind the socket to the end point */
	if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1){
		perror("bind");
		exit(1);
	}
	
	/* Start listening */
	if(listen(sockfd, BACKLOG) == -1){
		perror("listen");
		exit(1);
	}
	
	printf("server starts listening ... <3 \n");
	
	/* repeat: accept, send, close the connection */
	/* for every accepted connection, use a seperate process or thread to serve it */
	while(1){/* main accept() loop */
		sin_size = sizeof(struct sockaddr_in);
		if((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1){
			perror("accept");
			continue;
		}
		
		printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));
		
		
		if(!fork()){
	
			int *results = Receive_Array_Int_data(new_fd, 30);
		
			for(i=0; i<30;i++){
				printf("%d", results[i]);
				
			}
			
			
			

			Send_message(new_fd, "================================================================================\nWelcome to the online Minesweeper gaming system\n================================================================================\n\nYou are required to log on with your registered user name and password.\n\nUsername:");

			
			name = Recieve_message(new_fd);
			printf("%s",name);
			
			Send_message(new_fd, "Password: ");

			password = Recieve_message(new_fd);
			printf("%s",password);

			Send_message(new_fd, "Welcome to the Minesweeper gaming system.\n\nPlease enter a selection:\n<1> Play Minesweeper\n<2> Show Leaderboard\n<3> Quit\n\nSelect option (1-3):");

			
			char *option;
			option = Recieve_message(new_fd);
			int decision = atoi(option);
			
		
			if(decision == 3){
				Send_message(new_fd, "Thanks for playing!!\n");
				close(new_fd);
				exit(0);

			}
			if(decision == 2){
				Send_message(new_fd, "leaderboard selected");
				

			}
			if(decision == 1){
				Gamestate *currentgame = malloc(sizeof(Gamestate));
				place_mines(currentgame);
				char *showtile[9][9];
				
				int index = 0;
				char wow[2] = "\n";
		
				printf("\n");
				for(int j = 0; j < 9; j++){ //column
					for(int k = 0; k < 9; k++){ //row
						
						
						
						if(currentgame->tiles[k][j].revealed == false){
							showtile[k][j] = "P";
						}else{
							if(currentgame->tiles[k][j].is_mine == true){
								showtile[k][j] = "@";
							}else{
								if(currentgame->tiles[k][j].adjacent_mines == 0){
									showtile[k][j] = "0";
								}else if(currentgame->tiles[k][j].adjacent_mines == 1){
									showtile[k][j] = "1";
								} else if(currentgame->tiles[k][j].adjacent_mines == 2){
									showtile[k][j] = "2";
								}else if(currentgame->tiles[k][j].adjacent_mines == 3){
									showtile[k][j] = "3";
								}else{
									showtile[k][j] = "4";
								}

							}

						}
						printf("%s", showtile[k][j]);
						
					}	
					printf("%s", wow);
				}
				const char *gridstate[264] = {" ", " ", "1", "2", "3", "4","5","6","7","8","9", "\n",
										"-","-","-","-","-","-","-","-","-","-","-", "\n",
										"A", "|", showtile[0][0], showtile[1][0], showtile[2][0], showtile[3][0], showtile[4][0],showtile[5][0],showtile[6][0],showtile[7][0],showtile[8][0],"\n",
										 "B", "|", showtile[0][0], showtile[1][0], showtile[2][0], showtile[3][0], showtile[4][0],showtile[5][0],showtile[6][0],showtile[7][0],showtile[8][0],"\n",
										 "C", "|", showtile[0][0], showtile[1][0], showtile[2][0], showtile[3][0], showtile[4][0],showtile[5][0],showtile[6][0],showtile[7][0],showtile[8][0],"\n",
										 "D", "|", showtile[0][0], showtile[1][0], showtile[2][0], showtile[3][0], showtile[4][0],showtile[5][0],showtile[6][0],showtile[7][0],showtile[8][0],"\n",
										 "E", "|", showtile[0][0], showtile[1][0], showtile[2][0], showtile[3][0], showtile[4][0],showtile[5][0],showtile[6][0],showtile[7][0],showtile[8][0],"\n",
										 "F", "|", showtile[0][0], showtile[1][0], showtile[2][0], showtile[3][0], showtile[4][0],showtile[5][0],showtile[6][0],showtile[7][0],showtile[8][0],"\n",
										 "G", "|", showtile[0][0], showtile[1][0], showtile[2][0], showtile[3][0], showtile[4][0],showtile[5][0],showtile[6][0],showtile[7][0],showtile[8][0],"\n",
										 "I", "|", showtile[0][0], showtile[1][0], showtile[2][0], showtile[3][0], showtile[4][0],showtile[5][0],showtile[6][0],showtile[7][0],showtile[8][0],"\n"};
				for(int i = 0; i<132; i++){
					printf("%s", gridstate[i]);
				}
				//char lol[90] = ["1", "3", "2","3"];
				Send_message(new_fd, gridstate);
			}

			Send_message(new_fd, "You want to play!!\n");

			name = Recieve_message(new_fd);
			
			
			close(new_fd);
			exit(0);
			
	
		}
		close(new_fd);
	
			
	}
	

	


	for(int i = 0; i < NUM_TILES_X; i++){
		for(int j = 0; j < NUM_TILES_Y; j++){
			printf("%d", tester[i][j]);
		}
		printf("\n");
	
	}
	
	

	tester[5][4] = 9;
	printf("\n");
	printf("\n");
	for(int i = 0; i < NUM_TILES_X; i++){
		for(int j = 0; j < NUM_TILES_Y; j++){
			printf("%d", tester[i][j]);
		}
		printf("\n");
	
	}
	

	return EXIT_SUCCESS;
}
