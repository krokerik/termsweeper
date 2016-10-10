#include <stdlib.h>
#include <error.h>
#include <argp.h>
#include <ncurses.h>
#include <math.h>
#include <string.h>

/* Tile types */
#define EMPTY          0
#define MINE           1
#define FLAG_AND_EMPTY 2
#define FLAG_AND_MINE  3

const char *argp_program_version = "termsweeper 0.02";
const char *argp_program_bug_address = "<erik_andersson007@hotmail.com>";


/* Program documentation. */
static char doc[] = "termsweeper - a simple minesweeper clone in the\
 terminal.";

/* The options we understand. */
static struct argp_option options[] = {
  {"width",   'w', "WIDTH",  0,
   "Change number of columns in the game to WIDTH." },
  {"height",   'h', "HEIGHT",  0,
   "Change number of rows in the game to HEIGHT." },
  {"mines",   'm', "MINES",  0,
   "Set the number of mines to put on the board to MINES. MINES has to be\
 larger than 0 and smaller than HEIGHT*WIDTH." },
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments {
	int width, height, mine_count;
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state) {
	/* Get the input argument from argp_parse, which we
	 * know is a pointer to our arguments structure. */
	struct arguments *arguments = state->input;

	switch (key) {
		case 'w':
			arguments->width = arg ? atoi (arg) : 10;
			break;
		case 'h':
			arguments->height = arg ? atoi (arg) : 10;
			break;
		case 'm':
			arguments->mine_count = arg ? atoi (arg) : 10;
			break;
		case ARGP_KEY_ARG:
			state->next = state->argc;
			break;
		case ARGP_KEY_END:
			if(arguments->mine_count >= arguments->width*arguments->height)
				argp_error(state, "Number of mine has to be larger than 0 and\
 smaller than the width*height.");
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, 0, doc };

int isSolved(int *game, struct arguments args) {
	int flag_count = 0;
	for(int i=0; i<args.height; i++) {
		for(int j=0; j<args.width; j++) {
			switch(game[i*args.width + j]) {
				case FLAG_AND_EMPTY:
				case FLAG_AND_MINE:
					flag_count++;
					break;
				case EMPTY:
				case MINE:
				default:
					break;
			}
		}
	}
	return flag_count;
}

int nDigits(int integer) {
  return floor(log10(abs(integer))) + 1;
}

int nnDigits(int num, ...) {
  va_list valist;
  int sum = 0;
  va_start(valist, num);

  for(int i=0; i<num; i++) {
    sum += nDigits(va_arg(valist, int));
  }

  return sum;
}

int
main (int argc, char **argv)
{
	struct arguments arguments;

	/* Default values. */
	arguments.width = 80;
	arguments.height = 40;
	arguments.mine_count = 10;


	/* Parse our arguments; every option seen by parse_opt will be
	 * reflected in arguments. */
	argp_parse (&argp, argc, argv, 0, 0, &arguments);
  int row,col;
  char *msg;
  char format[] = "width %d, height %d, mines %d, scr rows %d, scr cols %d";
	initscr();
  do {
    clear();
    getmaxyx(stdscr,row,col);
    int len = strlen(format) + nnDigits(5,arguments.width, arguments.height,
                                        arguments.mine_count, row, col);
    msg = malloc(sizeof(char)*len);
    snprintf(msg, len, format, arguments.width, arguments.height,
             arguments.mine_count, row, col);
  	mvprintw (row/2,(col-strlen(msg))/2,msg);
  	refresh();
    free(msg);
  } while (getch() != 113);
	endwin();

	// parse flags
	// ask for input not provided at launch
	// generate empty matrix of correct size
	int *game;
	game = malloc(arguments.height*arguments.width*sizeof(int));
	for(int i=0; i<arguments.height; i++) {
		for(int j=0; j<arguments.width; j++) {
			game[i*arguments.width + j] = FLAG_AND_EMPTY;
		}
	}
	// start game loop
	while(!isSolved(game, arguments)) {
		int flag_count = 0;
		for(int i=0; i<arguments.height; i++) {
			for(int j=0; j<arguments.width; j++) {
				switch(game[i*arguments.width + j]) {
					case EMPTY:
						printf("0");
						break;
					case MINE:
						printf("*");
						break;
					case FLAG_AND_EMPTY:
						printf("1");
						break;
					case FLAG_AND_MINE:
						printf(".");
						break;
					default:
			      printf("?");
				}
			}
			printf("\n");
		}
		// draw ui
		// draw matrix
		// wait for first click and then place the mines
	}
	free(game);
  exit (0);
}
