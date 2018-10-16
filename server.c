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
#define RANDOM_NUMBER_SEED 42
#define R_Capital 82
#define P_Capital 80
#define Q_Capital 81

#define BACKLOG 10

int number_user = 0;

int tester[9][9];

struct user
{
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
	bool flag;
} Tile;

typedef struct
{
	int great;
	Tile tiles[NUM_TILES_X][NUM_TILES_Y];
} Gamestate;

/* Checks if tile contains a mine */
bool tile_contains_mine(int x, int y, Gamestate *currentstate)
{
	if (currentstate->tiles[x][y].is_mine == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*Fills grid with number of adjacent mines */
void number_of_adjacent_mines(Gamestate *currentstate)
{
	for (int i = 0; i < 9; i++)
	{ //row
		for (int j = 0; j < 9; j++)
		{ //column
			if (i - 1 >= 0)
			{
				if (currentstate->tiles[i - 1][j].is_mine == true)
				{
					currentstate->tiles[i][j].adjacent_mines++;
				}
			}
			if (j - 1 >= 0)
			{
				if (currentstate->tiles[i][j - 1].is_mine == true)
				{
					currentstate->tiles[i][j].adjacent_mines++;
				}
			}
			if (i + 1 < 9)
			{
				if (currentstate->tiles[i + 1][j].is_mine == true)
				{
					currentstate->tiles[i][j].adjacent_mines++;
				}
			}
			if (j + 1 < 9)
			{
				if (currentstate->tiles[i][j + 1].is_mine == true)
				{
					currentstate->tiles[i][j].adjacent_mines++;
				}
			}
			if (i - 1 >= 0 && j - 1 >= 0)
			{
				if (currentstate->tiles[i - 1][j - 1].is_mine == true)
				{
					currentstate->tiles[i][j].adjacent_mines++;
				}
			}
			if (i - 1 >= 0 && j + 1 < 9)
			{
				if (currentstate->tiles[i - 1][j + 1].is_mine == true)
				{
					currentstate->tiles[i][j].adjacent_mines++;
				}
			}
			if (i + 1 < 9 && j - 1 >= 0)
			{
				if (currentstate->tiles[i + 1][j - 1].is_mine == true)
				{
					currentstate->tiles[i][j].adjacent_mines++;
				}
			}
			if (i + 1 < 9 && j + 1 < 9)
			{
				if (currentstate->tiles[i + 1][j + 1].is_mine == true)
				{
					currentstate->tiles[i][j].adjacent_mines++;
				}
			}
		}
	}
}

/*Intialise game with mines */
void place_mines(Gamestate *currentstate)
{
	/*resetting gamestate*/
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			currentstate->tiles[i][j].is_mine = false;
			currentstate->tiles[i][j].revealed = false;
			currentstate->tiles[i][j].adjacent_mines = 0;
			currentstate->tiles[i][j].flag = false;
		}
	}

	for (int i = 0; i < NUM_MINES; i++)
	{
		int x, y;
		do
		{
			x = rand() % NUM_TILES_X;
			y = rand() % NUM_TILES_Y;
		} while (tile_contains_mine(x, y, currentstate));
		printf(" %d,%d ", x, y);
		currentstate->tiles[x][y].is_mine = true;
	}
	number_of_adjacent_mines(currentstate);
}

/*Recieves basic char message */
char *Recieve_message(int socket)
{
	char *buf = malloc(sizeof(char) * MAXDATASIZE);
	int numbytes;

	if ((numbytes = recv(socket, buf, MAXDATASIZE, 0)) == -1)
	{
		perror("recv");
		exit(1);
	}

	buf[numbytes] = '\0';
	buf[numbytes] = '\0';
	return buf;
}

/*Recieves basic number */
void Send_message(int socket, const void *message)
{
	if (send(socket, message, 1000, 0) == -1)
	{
		perror("send");
	}
}

/*Sends grid to client */
void Send_gamestate(int socket, Gamestate *currentstate)
{
	char *showtile[9][9];
	for (int j = 0; j < 9; j++)
	{ //column
		for (int k = 0; k < 9; k++)
		{ //row

			if (currentstate->tiles[k][j].revealed == false)
			{
				showtile[k][j] = " ";
			}
			else
			{
				if (currentstate->tiles[k][j].flag == true)
				{
					showtile[k][j] = "+";
				}
				else if (currentstate->tiles[k][j].is_mine == true)
				{
					showtile[k][j] = "@";
				}
				else
				{
					if (currentstate->tiles[k][j].adjacent_mines == 0)
					{
						showtile[k][j] = "0";
					}
					else if (currentstate->tiles[k][j].adjacent_mines == 1)
					{
						showtile[k][j] = "1";
					}
					else if (currentstate->tiles[k][j].adjacent_mines == 2)
					{
						showtile[k][j] = "2";
					}
					else if (currentstate->tiles[k][j].adjacent_mines == 3)
					{
						showtile[k][j] = "3";
					}
					else
					{
						showtile[k][j] = "4";
					}
				}
			}
		}
	}
	const char *gridstate[132] = {" ", " ", "1", "2", "3", "4", "5", "6", "7", "8", "9", "\n",
								  "-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "\n",
								  "A", "|", showtile[0][0], showtile[1][0], showtile[2][0], showtile[3][0], showtile[4][0], showtile[5][0], showtile[6][0], showtile[7][0], showtile[8][0], "\n",
								  "B", "|", showtile[0][1], showtile[1][1], showtile[2][1], showtile[3][1], showtile[4][1], showtile[5][1], showtile[6][1], showtile[7][1], showtile[8][1], "\n",
								  "C", "|", showtile[0][2], showtile[1][2], showtile[2][2], showtile[3][2], showtile[4][2], showtile[5][2], showtile[6][2], showtile[7][2], showtile[8][2], "\n",
								  "D", "|", showtile[0][3], showtile[1][3], showtile[2][3], showtile[3][3], showtile[4][3], showtile[5][3], showtile[6][3], showtile[7][3], showtile[8][3], "\n",
								  "E", "|", showtile[0][4], showtile[1][4], showtile[2][4], showtile[3][4], showtile[4][4], showtile[5][4], showtile[6][4], showtile[7][4], showtile[8][4], "\n",
								  "F", "|", showtile[0][5], showtile[1][5], showtile[2][5], showtile[3][5], showtile[4][5], showtile[5][5], showtile[6][5], showtile[7][5], showtile[8][5], "\n",
								  "G", "|", showtile[0][6], showtile[1][6], showtile[2][6], showtile[3][6], showtile[4][6], showtile[5][6], showtile[6][6], showtile[7][6], showtile[8][6], "\n",
								  "H", "|", showtile[0][7], showtile[1][7], showtile[2][7], showtile[3][7], showtile[4][7], showtile[5][7], showtile[6][7], showtile[7][7], showtile[8][7], "\n",
								  "I", "|", showtile[0][8], showtile[1][8], showtile[2][8], showtile[3][8], showtile[4][8], showtile[5][8], showtile[6][8], showtile[7][8], showtile[8][8], "\n"};
	for (int i = 0; i < 132; i++)
	{
		printf("%s", gridstate[i]);
		Send_message(socket, gridstate[i]);
	}
}

/* Checks to see how many active mines are still out there */
int Check_number_mines(int number_mines, Gamestate *currentstate)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (currentstate->tiles[i][j].is_mine == true)
			{
				number_mines++;
			}
			if (currentstate->tiles[i][j].flag == true && currentstate->tiles[i][j].is_mine == true)
			{
				number_mines--;
			}
		}
	}
	return number_mines;
}

/*Send int to client */
void send_int(int number, int socket)
{
	uint16_t statistics;

	statistics = htons(number);
	send(socket, &statistics, sizeof(uint16_t), 0);
}

/*Reveals tiles when tile in question is a zero value */
Gamestate *Reveal_function(Gamestate *currentstate, int x_cood, int y_cood)
{

	currentstate->tiles[x_cood][y_cood].revealed = true;
	int index = 100;
	while (index > 0)
	{

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (currentstate->tiles[i][j].revealed == true && currentstate->tiles[i][j].adjacent_mines == 0 && currentstate->tiles[i][j].is_mine == false)
				{
					if (i - 1 >= 0)
					{
						if (currentstate->tiles[i - 1][j].is_mine == false)
						{
							currentstate->tiles[i - 1][j].revealed = true;
						}
					}
					if (j - 1 >= 0)
					{
						if (currentstate->tiles[i][j - 1].is_mine == false)
						{
							currentstate->tiles[i][j - 1].revealed = true;
						}
					}
					if (i + 1 < 9)
					{
						if (currentstate->tiles[i + 1][j].is_mine == false)
						{
							currentstate->tiles[i + 1][j].revealed = true;
						}
					}
					if (j + 1 < 9)
					{
						if (currentstate->tiles[i][j + 1].is_mine == false)
						{
							currentstate->tiles[i][j + 1].revealed = true;
						}
					}
					if (i - 1 >= 0 && j - 1 >= 0)
					{
						if (currentstate->tiles[i - 1][j - 1].is_mine == false)
						{
							currentstate->tiles[i - 1][j - 1].revealed = true;
						}
					}
					if (i - 1 >= 0 && j + 1 < 9)
					{
						if (currentstate->tiles[i - 1][j + 1].is_mine == false)
						{
							currentstate->tiles[i - 1][j + 1].revealed = true;
						}
					}
					if (i + 1 < 9 && j - 1 >= 0)
					{
						if (currentstate->tiles[i + 1][j - 1].is_mine == false)
						{
							currentstate->tiles[i + 1][j - 1].revealed = true;
						}
					}
					if (i + 1 < 9 && j + 1 < 9)
					{
						if (currentstate->tiles[i + 1][j + 1].is_mine == false)
						{
							currentstate->tiles[i + 1][j + 1].revealed = true;
						}
					}
				}
			}
		}
		index--;
	}
	return currentstate;
}

/* Checks errors to see if it needs to send message to client */
int Error_checker(int error_number, int socket)
{
	if (error_number == 1)
	{
		Send_message(socket, "Already Revealed!");
		error_number = 0;
	}
	if (error_number == 2)
	{
		Send_message(socket, "Already Flagged!");
		error_number = 0;
	}
	if (error_number == 3)
	{
		Send_message(socket, "No mine!");
		error_number = 0;
	}
	return error_number;
}

/*main function */
int main(int argc, char **argv)
{
	srand(RANDOM_NUMBER_SEED);

	char *name;
	char *password;

	FILE *input_file = fopen("Authentication.txt", "r");
	int line = 0;

	char input[512];
	int letter;

	if (input_file == NULL)
	{
		perror("Error opening file");
		return (-1);
	}
	do
	{
		letter = fgetc(input_file);
		if (feof(input_file))
		{
			break;
		}
		/*do stuff here */
		printf("%c", letter);
	} while (1);
	/*
	rewind(input_file);

	while(fgets(input,512,input_file)){
		line++;
		printf("| %d -> %s |",line, input);
		
		

	}
	*/

	auth_users *reguser = malloc(sizeof(auth_users) * 20);
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

	int sockfd, new_fd;			   /*listen on sock_fd, new connection on new_fd */
	struct sockaddr_in my_addr;	/* mu address information */
	struct sockaddr_in their_addr; /* connector's address information */
	socklen_t sin_size;
	int i = 0;
	int socket_number;

	/* Get port number for server to listen on */
	if (argc != 2)
	{
		socket_number = 12345;
	}
	else
	{
		socket_number = atoi(argv[1]);
	}

	/* generate the socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	/*generate the socket */
	my_addr.sin_family = AF_INET;			 /*host byte order (internet domain)*/
	my_addr.sin_port = htons(socket_number); /* short, network byte order htons converts also assigning port number*/
	my_addr.sin_addr.s_addr = INADDR_ANY;	/* auto fill with my IP address */

	/* Bind the socket to the end point */
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}

	/* Start listening */
	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen");
		exit(1);
	}

	printf("server starts listening ... <3 \n");

	/* repeat: accept, send, close the connection */
	/* for every accepted connection, use a seperate process or thread to serve it */
	while (1)
	{ /* main accept() loop */
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
		{
			perror("accept");
			continue;
		}

		printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));

		if (!fork())
		{
			/*Sends introduction message */
			Send_message(new_fd, "================================================================================\nWelcome to the online Minesweeper gaming system\n================================================================================\n\nYou are required to log on with your registered user name and password.\n\nUsername:");

			/*Recieves users name */
			name = Recieve_message(new_fd);

			/*Asks for password */
			Send_message(new_fd, "Password: ");

			/* Recieves password*/
			password = Recieve_message(new_fd);

			/* Welcomes them to gaming system */
			Send_message(new_fd, "Welcome to the Minesweeper gaming system.\n\nPlease enter a selection:\n<1> Play Minesweeper\n<2> Show Leaderboard\n<3> Quit\n\nSelect option (1-3):");

			/*Turns option into int */
			char *option;
			option = Recieve_message(new_fd);
			int decision = atoi(option);

			/*Quit*/
			if (decision == 3)
			{
				Send_message(new_fd, "Thanks for playing!!\n");
				close(new_fd);
				exit(0);
			}

			/* Leaderboard */
			if (decision == 2)
			{
				Send_message(new_fd, "leaderboard selected");
			}
			if (decision == 1)
			{
				/*intialising game */
				bool playing = true;
				Gamestate *currentgame = malloc(sizeof(Gamestate));
				place_mines(currentgame);
				int problem = 0;

				/*starting game */
				while (playing == true)
				{

					Send_message(new_fd, "Remaining mines: ");
					int number_mines = 0;
					number_mines = Check_number_mines(number_mines, currentgame);
					send_int(number_mines, new_fd);

					Send_gamestate(new_fd, currentgame);
					option = Recieve_message(new_fd);
					int move = (int)option[0];
					/*Reveal Option */
					if (move == R_Capital)
					{
						char *coordinate;
						int x_cood;
						int y_cood;
						coordinate = Recieve_message(new_fd);
						y_cood = (int)coordinate[0] - 65;
						x_cood = (int)coordinate[1] - 49;
						if (currentgame->tiles[x_cood][y_cood].is_mine == true)
						{
							currentgame->tiles[x_cood][y_cood].revealed = true;
							problem = 9;
						}
						else if (currentgame->tiles[x_cood][y_cood].revealed == true)
						{
							problem = 1;
						}
						else
						{
							currentgame = Reveal_function(currentgame, x_cood, y_cood);
						}
					}
					/*Place flag option */
					if (move == P_Capital)
					{
						char *coordinate;
						int x_cood;
						int y_cood;
						coordinate = Recieve_message(new_fd);
						y_cood = (int)coordinate[0] - 65;
						x_cood = (int)coordinate[1] - 49;
						if (currentgame->tiles[x_cood][y_cood].flag == true)
						{
							problem = 2;
						}
						else if (currentgame->tiles[x_cood][y_cood].is_mine == true)
						{
							currentgame->tiles[x_cood][y_cood].revealed = true;
							currentgame->tiles[x_cood][y_cood].flag = true;
							if (number_mines - 1 == 0)
							{
								problem = 9;
							}
						}
						else
						{
							problem = 3;
						}
					}
					if (move == Q_Capital)
					{
						problem = 9;
					}

					send_int(problem, new_fd);

					problem = Error_checker(problem, new_fd);

					/*End game*/
					if (problem == 9)
					{
						Send_gamestate(new_fd, currentgame);
						Send_message(new_fd, "End of Game");
						playing = false;
					}
				}
			}

			close(new_fd);
			exit(0);
		}
		close(new_fd);
	}

	return EXIT_SUCCESS;
}
