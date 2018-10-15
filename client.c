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
} Gamestate;

void Send_Array_Data(int socket_id, int *myArray)
{
	int i = 0;
	uint16_t statistics;

	for (i = 0; i < 30; i++)
	{
		statistics = htons(myArray[i]);
		send(socket_id, &statistics, sizeof(uint16_t), 0);
	}
}

void Recieve_message(int socket)
{
	char buf[MAXDATASIZE];
	int numbytes;

	if ((numbytes = recv(socket, buf, MAXDATASIZE, 0)) == -1)
	{
		perror("recv");
		exit(1);
	}

	buf[numbytes] = '\0';
	buf[numbytes] = '\0';

	printf("\n%s", buf);
}
void Recieve_message_no_line(int socket)
{
	char buf[MAXDATASIZE];
	int numbytes;

	if ((numbytes = recv(socket, buf, MAXDATASIZE, 0)) == -1)
	{
		perror("recv");
		exit(1);
	}

	buf[numbytes] = '\0';
	buf[numbytes] = '\0';

	printf("%s", buf);
}

int main(int argc, char **argv)
{

	Gamestate *minegame = malloc(sizeof(Gamestate));
	int sockfd, numbytes, i = 0;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information */

	if (argc != 3)
	{
		fprintf(stderr, "usage: client_hostname port_number\n");
		exit(1);
	}

	if ((he = gethostbyname(argv[1])) == NULL)
	{ /* get host info */
		herror("gethostbyname");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;			/* host byte order */
	their_addr.sin_port = htons(atoi(argv[2])); /*short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8); /* zero the rest of the stuct */

	if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("connect");
		exit(1);
	}

	int simpleArray[30] = {0};
	for (i = 0; i < 30; i++)
	{
		simpleArray[i] = i * i;
		printf("%d", simpleArray[i]);
	}

	Send_Array_Data(sockfd, simpleArray);

	Recieve_message(sockfd);

	char message[1000];
	/*username send*/
	scanf("%s", message);
	if (send(sockfd, message, strlen(message), 0) == -1)
	{
		perror("send");
	}

	Recieve_message(sockfd);

	scanf("%s", message);
	if (send(sockfd, message, strlen(message), 0) == -1)
	{
		perror("send");
	}

	Recieve_message(sockfd);

	int decision;
	while (1)
	{
		scanf("%s", message);
		decision = atoi(message);
		if (decision == 1 || decision == 2 || decision == 3)
		{
			break;
		}
		printf("\nPlease select one of the three options: \n");
	}

	if (send(sockfd, message, strlen(message), 0) == -1)
	{
		perror("send");
	}

	if (decision == 3)
	{
		Recieve_message(sockfd);
		close(sockfd);
		return EXIT_SUCCESS;
	}

	if (decision == 1)
	{
		for (int i = 0; i < 132; i++)
		{
			Recieve_message_no_line(sockfd);
		}

		printf("Choose an option:\n");
		printf("<R> Reveal flag\n");
		printf("<P> Place flag\n");
		printf("<Q> Quit game\n\n");
		printf("Option (R,P,Q): ");
		int new;
		while (1)
		{
			scanf("%s", message);
			printf("%c,%c", message[0], message[1]);
			new = (int)message[0];
			if (new == 80 || new == 81 || new == 82)
			{
				break;
			}
			printf("Please Try again: ");
		}
		if (send(sockfd, message, strlen(message), 0) == -1)
		{
			perror("send");
		}
		if (new == 82)
		{
			while (1)
			{
				int letter;
				int number;
				bool correctletter = false;
				printf("\nEnter tile coordinate: ");
				scanf("%s", message);
				letter = (int)message[0];
				number = (int)message[1];
				if (65 <= letter && letter <= 73)
				{
					correctletter = true;
				}
				if (49 <= number && number <= 57 && correctletter == true)
				{
					break;
				}
			}
			if (send(sockfd, message, strlen(message), 0) == -1)
			{
				perror("send");
			}
		}
	}

	for (int i = 0; i < 132; i++)
	{
		Recieve_message_no_line(sockfd);
	}


	close(sockfd);

	return EXIT_SUCCESS;
}
