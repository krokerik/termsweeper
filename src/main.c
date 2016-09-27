#include <stdlib.h>
#include <error.h>
#include <argp.h>

const char *argp_program_version = "termsweeper 0.01";
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

	printf ("width\t%d\n", arguments.width);
	printf ("height\t%d\n", arguments.height);
	printf ("mines\t%d\n", arguments.mine_count);
	
	
	// parse flags
	// ask for input not provided at launch
	// generate empty matrix of correct size
	// start game loop
	//   draw ui
	//   draw matrix
	//   wait for first click and then place the mines

  exit (0);
}