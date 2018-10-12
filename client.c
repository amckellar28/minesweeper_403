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


void Send_Array_Data(int socket_id, int *myArray){
	int i =0;
	uint16_t statistics;
	
	for(i=0; i< 30; i++){
		statistics = htons(myArray[i]);
		send(socket_id, &statistics, sizeof(uint16_t),0);
	}
}

void Recieve_message(int socket){
	char buf[MAXDATASIZE];
	int numbytes;
	
	if((numbytes = recv(socket, buf, MAXDATASIZE,0))==-1){
		perror("recv");
		exit(1);
	}
	
	buf[numbytes] = '\0';
	buf[numbytes] = '\0';
	
	printf("\n%s",buf);

}
	



int main(int argc, char **argv){
	int sockfd, numbytes, i=0;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information */
	
	if(argc != 3){
		fprintf(stderr, "usage: client_hostname port_number\n");
		exit(1);
	}
	
	if((he = gethostbyname(argv[1])) == NULL){ /* get host info */
		herror("gethostbyname");
		exit(1);
	}
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket");
		exit(1);
	}
	
	their_addr.sin_family = AF_INET; /* host byte order */
	their_addr.sin_port = htons(atoi(argv[2])); /*short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8); /* zero the rest of the stuct */
	
	if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1){
		perror("connect");
		exit(1);
	}
	
	int simpleArray[30] = {0};
	for(i=0; i < 30; i++){
		simpleArray[i] = i*i;
		printf("%d", simpleArray[i]);
	}
	
	Send_Array_Data(sockfd, simpleArray);
	
	Recieve_message(sockfd);
	
	char message[1000];
	/*username send*/
	scanf("%s", message);
	if(send(sockfd, message,strlen(message),0)==-1){
				perror("send");
			}
	

	Recieve_message(sockfd);

	scanf("%s", message);
	if(send(sockfd, message,strlen(message),0)==-1){
				perror("send");
			}
	
	Recieve_message(sockfd);
	
	

	
	close(sockfd);
	
	
	
	



	Tile alice = {6006,true,false};
	Gamestate wow;
	
	
	
	

	int board[NUM_TILES_X][NUM_TILES_Y];
	
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			board[i][j] = 0;
		}
	}
	
	
	printf("     1 2 3 4 5 6 7 8 9\n");
	printf("----------------------\n");
	printf(" A | %d %d %d %d %d %d %d %d %d\n",board[0][0],board[0][1],board[0][2],board[0][3],board[0][4],board[0][5],board[0][6],
		board[0][7],board[0][8]);
	printf(" B | %d %d %d %d %d %d %d %d %d\n",board[1][0],board[1][1],board[1][2],board[1][3],board[1][4],board[1][5],board[1][6],
		board[1][7],board[1][8]);
	printf(" C | %d %d %d %d %d %d %d %d %d\n",board[2][0],board[2][1],board[2][2],board[2][3],board[2][4],board[2][5],board[2][6],
		board[2][7],board[2][8]);
	printf(" D | %d %d %d %d %d %d %d %d %d\n",board[3][0],board[3][1],board[3][2],board[3][3],board[3][4],board[3][5],board[3][6],
		board[3][7],board[3][8]);
	printf(" E | %d %d %d %d %d %d %d %d %d\n",board[4][0],board[4][1],board[4][2],board[4][3],board[4][4],board[4][5],board[4][6],
		board[4][7],board[4][8]);
	printf(" F | %d %d %d %d %d %d %d %d %d\n",board[5][0],board[5][1],board[5][2],board[5][3],board[5][4],board[5][5],board[5][6],
		board[5][7],board[5][8]);
	printf(" G | %d %d %d %d %d %d %d %d %d\n",board[6][0],board[6][1],board[6][2],board[6][3],board[6][4],board[6][5],board[6][6],
		board[6][7],board[6][8]);
	printf(" H | %d %d %d %d %d %d %d %d %d\n",board[7][0],board[7][1],board[7][2],board[7][3],board[7][4],board[7][5],board[7][6],
		board[7][7],board[7][8]);
	printf(" I | %d %d %d %d %d %d %d %d %d\n",board[8][0],board[8][1],board[8][2],board[8][3],board[8][4],board[8][5],board[8][6],
		board[8][7],board[8][8]);
	


	



	return EXIT_SUCCESS;
}
