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

#define NUM_TILES_X 9
#define NUM_TILES_Y 9
#define NUM_MINES 10

#define BACKLOG 10

int number_user=0;

int tester[9][9];

bool tile_contains_mine(int x, int y){
	if(tester[x][y] == 1){
		return true;
	} else{
		return false;
	}


}

void place_mines()
{
	for(int i = 0; i < NUM_MINES; i++)
	{
		int x, y;
		do{
			x = rand() % NUM_TILES_X;
			y = rand() % NUM_TILES_Y;
			printf("(%d,%d)",x,y);
		} while(tile_contains_mine(x, y));
			tester[x][y] = 1;
		
	
		
	}
}

int main(int argc, char **argv){
	FILE *input_file = fopen("Authentication.txt","r");
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
		*/
	}
	
	
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


	for(int i = 0; i < NUM_TILES_X; i++){
		for(int j = 0; j < NUM_TILES_Y; j++){
			printf("%d", tester[i][j]);
		}
		printf("\n");
	
	}
	
	
	place_mines();
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
