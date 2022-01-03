/*Source: A2.c 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"

void checkCardFile(char* cardFileName, FILE *fp);
void checkAmountOfArguments(int argc, char *argv[]);
void checkSeed(char *argv[]);
void checkCardFormat(char* cardFileName, FILE *fp);
int isCardFileSpaceLocation(int x);
int isCardFileNewlineLocation(int x);
int isCardFileDigitLocation(int x);
void checkBoardNums(char* cardFileName, int board[]);
int generateUniqueCall(list *callList);
void printBoard(int board[], int marked[]);
void markBoard(int board[], int marked[], int x);
int checkIfWon(int marked[]);

int main(int argc, char *argv[]) {
	checkAmountOfArguments(argc, argv);
	checkSeed(argv);
	srand(atoi(argv[1]));
	
	//getting the name of the card file
	int cardFileNameLength = strlen(argv[2]);
	char cardFileName[cardFileNameLength];
	strcpy(cardFileName, argv[2]); 

	//opening and checking the card file
	FILE *fp;
	fp = fopen(cardFileName,"r");
	checkCardFile(cardFileName, fp);
	checkCardFormat(cardFileName, fp);	
	
	//putting the game card numbers into an array	
	int boardSize = 25;
	int board[boardSize];
	int marked[boardSize];
	int i;
	for (i=0; i<25; i++) {
		fscanf(fp, "%d", &board[i]);
		marked[i] = 0;
	}
	fclose(fp);
	marked[12] = 1;

	checkBoardNums(cardFileName, board);

	//game loop
	char input;
	int inputSize;
	int numToCall;
	int won = 0;
	list *callList = initList(callList, 75);
	while (1) {
		system("clear");
		
		printf("Call list: ");	
		printCallList(callList);
		putchar('\n');
		printBoard(board, marked);
		
		//printf("%d\n", rand() % 75 + 1);
		if (!won) printf("enter any non-enter key for Call (q for quit): ");
		else {
			printf("WINNER\n");
			exit(0);
		}

		inputSize = 0;
		while (1) {
			input = getchar();
					
			if ((int)input == 10) {
				//when they input an enter key, then loop the amount chars before the
				//enter, then add to the call list, mark the board, and check if won.
				for (i=0; i<inputSize; i++) {
					numToCall = generateUniqueCall(callList);
					addToList(callList, numToCall);
					markBoard(board, marked, numToCall);
					if (checkIfWon(marked)) {
						won = 1;
						break;
					}
				}
				break;
			}
			else if (input == 'q') exit(0);
			else inputSize++;
		}
	}

	exit(0);
}
void checkCardFile(char* cardFileName, FILE *fp) {
        if (fp == NULL) {
                fprintf(stderr,"%s is nonexistent or unreadable\n",cardFileName);
                exit(3);
        }
}
void checkAmountOfArguments(int argc, char *argv[]) {
        if (argc != 3) {
                fprintf(stderr, "Usage: %s seed cardFile\n", argv[0]);
                exit(1);
        }
}
void checkSeed(char *argv[]) {
        int i;
        for (i = 0; i < strlen(argv[1]); i++) {
                if (!(isdigit(argv[1][i]))) {
                        fprintf(stderr, "Expected integer seed, but got %s\n", argv[1]);
                        exit(2);
                }
        }
}
void checkCardFormat(char* cardFileName, FILE *fp) {
        int ch;
        int i = 0;
        while (( ch = fgetc(fp)) != EOF) {
                //fputc(ch,stdout);
                if (!((ch == '\n' && (isCardFileNewlineLocation(i))) || (ch == ' ' && (isCardFileSpaceLocation(i))) || (isdigit(ch) && (isCardFileDigitLocation(i))))) {
                        fprintf(stderr, "%s has bad format\n", cardFileName);
                        exit(4);
                }
                i++;
        }
        rewind(fp);
}
int isCardFileSpaceLocation(int x) {
        int i;
        int spaceLocations[] = {2, 5, 8, 11, 17, 20, 23, 26, 32, 35, 38, 41, 47, 50, 53, 56, 62, 65, 68, 71};
        for (i=0; i<(sizeof(spaceLocations)/sizeof(spaceLocations[0])); i++) {
                if (x == spaceLocations[i]) return 1;
        }
        return 0;
}
int isCardFileNewlineLocation(int x) {
        int i;
        int newlineLocations[] = {14, 29, 44, 59, 74};
        for (i=0; i<(sizeof(newlineLocations)/sizeof(newlineLocations[0])); i++) {
                if (x == newlineLocations[i]) return 1;
        }
        return 0;
}
int isCardFileDigitLocation(int x) {
        if (x <= 74 && !(isCardFileSpaceLocation(x)) && !(isCardFileNewlineLocation(x))) return 1;
        return 0;
}
void checkBoardNums(char* cardFileName, int board[]) {
        //checking if any nums are repeated
        int i;
        int j;
        for (i=0; i<25; i++) {
                for (j=0; j<25; j++) {
                        if (i != j && board[i] == board[j]) {
                                fprintf(stderr, "%s has bad format\n", cardFileName);
                                exit(4);
                        }
                }
        }

        //checking if board numbers are in the right ranges
        for (i=0; i<25; i++) {
                if ((i % 5 == 0 && (board[i] < 1 || board[i] > 15)) || ((i-1) % 5 == 0 && (board[i] < 16 || board[i] > 30)) || (i != 12 && (i-2) % 5 == 0 && (board[i] < 31 || board[i] > 45)) || (i == 12 && board[i] != 0) || ((i-3) % 5 == 0 && (board[i] < 46 || board[i] > 60)) ||((i-4) % 5 == 0 && (board[i] < 61 || board[i] > 75))) {
                        fprintf(stderr, "%s has bad format\n", cardFileName);
                        exit(4);
                }
        }
}
int generateUniqueCall(list *callList) {
        int x;
        while (1) {
                x = (rand() % 75) + 1;
                if (!(listContains(callList, x))) return x;
        }
}
void printBoard(int board[], int marked[]) {
        int i;
        int boardSize = 25;
        char suffix;
        char prefix;
        printf("L   I   N   U   X\n");
        for (i=0; i<boardSize; i++) {
                if (marked[i]) suffix = 'm';
                else suffix = ' ';

                if (board[i] < 10) prefix = '0';
                else prefix = '\0';

                if ((i+1) % 5 == 0) printf("%c%d%c\n", prefix, board[i], suffix);
                else if (i == 12) printf("%c%d%c ", prefix, board[i], suffix);
                else printf("%c%d%c ", prefix, board[i], suffix);
        }
}
void markBoard(int board[], int marked[], int x) {
        int i;
        int boardSize = 25;
        for (i=0; i<boardSize; i++) {
                if (board[i] == x) {
                        marked[i] = 1;
                        return;
                }
        }
}
int checkIfWon(int marked[]) {
        //check four corners
        if (marked[0] && marked[4] && marked[20] && marked[24]) return 1;

        //check rows
        int i;
        int counter;
        for (i=0; i<25; i++) {
                if (i % 5 == 0) counter = 0;
                if (marked[i]) counter++;
                if (counter == 5) return 1;
        }

        //check columns
        int j;
        for (i=0; i<5; i++) {
                counter=0;
                for (j=i; j<=(i+20); j+=5) {
                        if (marked[j]) counter++;
                        if (counter == 5) return 1;
                }
        }
        return 0;
}
