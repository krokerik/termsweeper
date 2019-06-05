#include <stdlib.h>
#include <error.h>
#include <argp.h>
#include <ncurses.h>
#include <math.h>
#include <string.h>
#include <time.h>

/* Tile types */
#define EMPTY          0
#define MINE           1

/* default values */
#define DEFAULT_W 80
#define DEFAULT_H 40
#define DEFAULT_M DEFAULT_H*DEFAULT_W/320

const char *argp_program_version = "termsweeper 0.03";
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
	int upper;
	int lower;
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
			lower = 0;
			upper = arguments->width*arguments->height;
			if(arguments->mine_count >= upper || arguments->mine_count <= lower) {
				argp_error(state, "Number of mine has to be larger than 0 and \
smaller than the width*height.");
			}
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

int* initGame(struct arguments args) {
	int *game;
	int h = args.height;
	int w = args.width;
	int m = args.mine_count;
	int numValid = h*w;
	int* validMine = malloc(numValid*sizeof(int));
	for(int i=0; i<h*w; i++) {
		validMine[i] = i;
	}
	game = malloc(h*w*sizeof(int));
	for(int i=0; i<h; i++) {
		for(int j=0; j<w; j++) {
			game[i*w + j] = EMPTY;
		}
	}
	for(int i=0; i<m; i++) {
		int pos = rand()%numValid;
		game[validMine[pos]] = MINE;
		for(int j=pos; j<numValid; j++) {
			validMine[j] = validMine[j+1];
		}
		numValid--;
	}
	free(validMine);
	return game;
}

int main (int argc, char **argv)
{
	srand(time(NULL));
	struct arguments arguments;

	/* Default values. */
	arguments.width = DEFAULT_W;
	arguments.height = DEFAULT_H;
	arguments.mine_count = DEFAULT_M;


	/* Parse our arguments; every option seen by parse_opt will be
	 * reflected in arguments. */
	argp_parse (&argp, argc, argv, 0, 0, &arguments);
	int row,col;
	int *game = initGame(arguments);
	int *flags = malloc(1*sizeof(int));
	int numFlags = 0;
	char *msg;
	char format[] = "width %d, height %d, mines %d, scr rows %d, scr cols %d";
	initscr();
	do {
		clear();
		getmaxyx(stdscr,row,col);
		int len = strlen(format) + nnDigits(5,arguments.width, arguments.height,
			arguments.mine_count, row, col), centery = row/2, centerx = col/2;
		if(col<arguments.width || row<arguments.height) {
			len = strlen("size of window too small, ensure %dx%dx") + nnDigits(2,
				arguments.width,arguments.height);
			msg = malloc(sizeof(char)*len);
			snprintf(msg,len,"size of window too small, ensure %dx%d",arguments.width, arguments.height);
			mvprintw (centery,centerx-strlen(msg)/2,msg);
		} else {
			msg = malloc(sizeof(char)*len);
			//snprintf(msg, len, format, arguments.width, arguments.height,
			//	arguments.mine_count, row, col);
			for(int i=-1; i<arguments.height+1; i++) {
				for(int j=-1; j<arguments.width+1; j++) {
					if(i==-1||i==arguments.height||j==-1||j==arguments.width) {
						mvaddch(centery-arguments.height/2+i,centerx-arguments.width/2+j,254);
					} else {
						switch(game[i*arguments.width + j]) {
							case EMPTY:
								mvaddch(centery-arguments.height/2+i,centerx-arguments.width/2+j,46);
								break;
							case MINE:
								mvaddch(centery-arguments.height/2+i,centerx-arguments.width/2+j,42);
								break;
							default:
								break;
						}
					}
				}
			}
			for(int i=0; i<numFlags; i++) {
				int x=flags[i]%arguments.width;
				int y=flags[i]/arguments.width;
				mvaddch(centery-arguments.height/2+y,centerx-arguments.width/2+x,49);
			}
		}
		refresh();
		free(msg);
	} while (getch() != 113);
	endwin();
	free(game);
	free(flags);
	exit (0);
}
