/**
* @file core_rsh.cpp
*
* core_rsh
*
*   core_rsh.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"
#include "core_getopt.h"

/*
Function Declarations for builtin shell commands:
*/
int rsh_testopt(int argc, char *argv[]);
int rsh_help(int argc, char *argv[]);
int rsh_python(int argc, char *argv[]);
int rsh_version(int argc, char *argv[]);
int rsh_exit(int argc, char *argv[]);

//
int rsh_core_commander_main(int argc, char *argv[]);

/*
List of builtin commands, followed by their corresponding functions.
*/
char *builtin_str[] = {
	//
	"example",
	"commander",
	"rs485p1",
	"rs485p2", // Added by Joshua
	//
    "testopt",
    "help",
	"python",
	"version",
    "exit"
};

int(*builtin_func[]) (int argc, char *argv[]) = {
	//
	&rsh_core_example_main,
	&rsh_core_commander_main,
	&rsh_rs485p1_main,
	&rsh_rs485p2_main, // Added by Joshua
	//
	&rsh_testopt,
    &rsh_help,
	&rsh_interpreter_main,
	&rsh_version,
    &rsh_exit
};

int rsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

/*
Builtin function implementations.
*/

/**
@brief Bultin command: change directory.
@param argv List of argv.  argv[0] is "cd".  argv[1] is the directory.
@return Always returns 1, to continue executing.
*/
int rsh_testopt(int argc, char *argv[])
{
	printf("argc = %d\n", argc);

	for(int i = 0; i < argc; i++)
		printf("argv[%d] = %s\n", i, argv[i]);

	optind = 0;

#if 0
	if (argc < 2) {
		log_info("missing command");
		return 1;
	}

	if (!strcmp(argv[1], "start")) {
		log_info("start");
		return 0;
	}

	if (!strcmp(argv[1], "stop")) {
		log_info("stop");
		return 0;
	}

	if (!strcmp(argv[1], "status")) {
		log_info("status");
		return 0;
	}
#endif

#if 0
	int aflag = 0;
	int bflag = 0;
	char *cvalue = NULL;
	int index;
	int c;

	opterr = 0;

	while ((c = getopt(argc, argv, "abc:")) != -1)
		switch (c)
	{
		case 'a':
			printf("a\n");
			aflag = 1;
			break;
		case 'b':
			printf("b\n");
			bflag = 1;
			break;
		case 'c':
			printf("c\n");
			cvalue = optarg;
			break;
		case '?':
			printf("?\n");
			if (optopt == 'c')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr,
				"Unknown option character `\\x%x'.\n",
				optopt);
			//return 1;
			break;
		default:
			printf("about\n");
			abort();
			break;
	}

	printf("aflag = %d, bflag = %d, cvalue = %s\n",
		aflag, bflag, cvalue);

	for (index = optind; index < argc; index++)
		printf("Non-option argument %s\n", argv[index]);
	return 0;
#endif

#if 0
	bool amend = false;
	bool brief = false;
	const char *color = "white";
	int delay = 0;

	char *arg;
	int option;
	struct optparse options;

	optparse_init(&options, argv);
	while ((option = optparse(&options, "abc:d::")) != -1) {
		switch (option) {
		case 'a':
			amend = true;
			printf("%d\n", amend);
			break;
		case 'b':
			brief = true;
			printf("%d\n", brief);
			break;
		case 'c':
			color = options.optarg;
			printf("%d\n", options.optarg);
			break;
		case 'd':
			delay = options.optarg ? atoi(options.optarg) : 1;
			break;
		case '?':
			fprintf(stderr, "%s: %s\n", argv[0], options.errmsg);
			exit(EXIT_FAILURE);
		}
	}

	/* Print remaining arguments. */
	while ((arg = optparse_arg(&options)))
		printf("%s\n", arg);
	return 0;
#endif

#if 0
	/* Flag set by ．--verbose・. */
	static int verbose_flag;

	int c;

	while (1)
	{
		static struct option long_options[] =
		{
			/* These options set a flag. */
			{ "verbose", no_argument, &verbose_flag, 1 },
			{ "brief", no_argument, &verbose_flag, 0 },
			/* These options don・t set a flag.
			We distinguish them by their indices. */
			{ "add", no_argument, 0, 'a' },
			{ "append", no_argument, 0, 'b' },
			{ "delete", required_argument, 0, 'd' },
			{ "create", required_argument, 0, 'c' },
			{ "file", required_argument, 0, 'f' },
			{ 0, 0, 0, 0 }
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long(argc, argv, "abc:d:f:",
			long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
		case 0:
			/* If this option set a flag, do nothing else now. */
			if (long_options[option_index].flag != 0)
				break;
			printf("option %s", long_options[option_index].name);
			if (optarg)
				printf(" with arg %s", optarg);
			printf("\n");
			break;

		case 'a':
			puts("option -a\n");
			break;

		case 'b':
			puts("option -b\n");
			break;

		case 'c':
			printf("option -c with value `%s'\n", optarg);
			break;

		case 'd':
			printf("option -d with value `%s'\n", optarg);
			break;

		case 'f':
			printf("option -f with value `%s'\n", optarg);
			break;

		case '?':
			/* getopt_long already printed an error message. */
			break;

		default:
			;// abort();
		}
	}

	/* Instead of reporting ．--verbose・
	and ．--brief・ as they are encountered,
	we report the final status resulting from them. */
	if (verbose_flag)
		puts("verbose flag is set");

	/* Print any remaining command line arguments (not options). */
	if (optind < argc)
	{
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		putchar('\n');
	}
#endif

#if 1
	int opt = 0;
	int area = -1, perimeter = -1, breadth = -1, length = -1;

	//Specifying the expected options
	//The two options l and b expect numbers as argument
	static struct option long_options[] = {
		{ "area", no_argument, 0, 'a' },
		{ "perimeter", no_argument, 0, 'p' },
		{ "length", required_argument, 0, 'l' },
		{ "breadth", required_argument, 0, 'b' },
		{ 0, 0, 0, 0 }
	};

	int long_index = 0;
	while ((opt = getopt_long(argc, argv, "apl:b:",
		long_options, &long_index)) != -1) {
		switch (opt) {
		case 'a': area = 0;
			break;
		case 'p': perimeter = 0;
			break;
		case 'l': length = atoi(optarg);
			break;
		case 'b': breadth = atoi(optarg);
			break;
		//default: print_usage();
		//	exit(EXIT_FAILURE);
		default:
			break;
		}
	}
	if (length == -1 || breadth == -1) {
		//print_usage();
		//exit(EXIT_FAILURE);
	}

	// Calculate the area
	if (area == 0) {
		area = length * breadth;
		printf("Area: %d\n", area);
	}

	// Calculate the perimeter
	if (perimeter == 0) {
		perimeter = 2 * (length + breadth);
		printf("Perimeter: %d\n", perimeter);
	}
	return 0;
#endif

	return 0;
}

/**
@brief Builtin command: print help.
@param argv List of argv.  Not examined.
@return Always returns 1, to continue executing.
*/
int rsh_help(int argc, char *argv[])
{
    int i;
    printf("Ricardo's RSH\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < rsh_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int rsh_command(const char *cmd);
int rsh_version(int argc, char *argv[])
{
	//printf("rsh_version\n");
	/* MQTT */
	int mq_maj, mq_min, mq_rev;
	//char cmd[] = "testopt --area";

	if (argc < 2) {
		log_info("missing command");
		return 0;
	}

	if (!strcmp(argv[1], "all")) {
		mosquitto_lib_version(&mq_maj, &mq_min, &mq_rev);
		printf("mosquitto version %d.%d.%d\n", mq_maj, mq_min, mq_rev);
		//printf("libmodbus version %s (%06X)\n", LIBMODBUS_VERSION_STRING, LIBMODBUS_VERSION_HEX);
		printf("libmodbus version %s\n", LIBMODBUS_VERSION_STRING);

		return 0;
	}

	if (!strcmp(argv[1], "libmodbus")) {
		printf("libmodbus version %s\n", LIBMODBUS_VERSION_STRING);

		return 0;
	}

	if (!strcmp(argv[1], "mosquitto")) {
		mosquitto_lib_version(&mq_maj, &mq_min, &mq_rev);
		printf("mosquitto version %d.%d.%d\n", mq_maj, mq_min, mq_rev);

		return 0;
	}

	//rsh_command(cmd);

	return 0;
}

/**
@brief Builtin command: exit.
@param argv List of argv.  Not examined.
@return Always returns 0, to terminate execution.
*/
int rsh_exit(int argc, char *argv[])
{
    return 0;
}

/**
@brief Launch a program and wait for it to terminate.
@param argv Null terminated list of arguments (including program).
@return Always returns 1, to continue execution.
*/
int rsh_launch(int argc, char *argv[])
{
#if 0
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(argv[0], argv) == -1) {
            perror("rsh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0) {
        // Error forking
        perror("rsh");
    }
    else {
        // Parent process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
#else
	printf("error : not a build-in command!\n");
#endif

    return 0;
}

/**
@brief Execute shell built-in or launch program.
@param argv Null terminated list of arguments.
@return 1 if the shell should continue running, 0 if it should terminate
*/
int rsh_execute(int argc, char *argv[])
{
    int i;

    if (argv[0] == NULL) {
        // An empty command was entered.
        return 0;
    }

    for (i = 0; i < rsh_num_builtins(); i++) {
        if (strcmp(argv[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(argc, argv);
        }
    }

    return rsh_launch(argc, argv);
}

#define RSH_RL_BUFSIZE 1024
/**
@brief Read a line of input from stdin.
@return The line from stdin.
*/
char *rsh_read_line(void)
{
    int bufsize = RSH_RL_BUFSIZE;
    int position = 0;
    char *buffer = (char *)malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "rsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Read a character
        c = getchar();

        if (c == EOF) {
            exit(EXIT_SUCCESS);
        }
        else if (c == '\n') {
            buffer[position] = '\0';
            return buffer;
        }
        else {
            buffer[position] = c;
        }
        position++;

        // If we have exceeded the buffer, reallocate.
        if (position >= bufsize) {
            bufsize += RSH_RL_BUFSIZE;
            buffer = (char *)realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "rsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        //printf("position = %d \n", position);
    }
}

#define RSH_TOK_BUFSIZE 64
#define RSH_TOK_DELIM " \t\r\n\a"
/**
@brief Split a line into tokens (very naively).
@param line The line.
@return Null-terminated array of tokens.
*/
char **rsh_split_line(char *line, int *argc)
{
    int bufsize = RSH_TOK_BUFSIZE, position = 0;
    char **tokens = (char **)malloc(bufsize * sizeof(char*));
    char *token, **tokens_backup;

    if (!tokens) {
        fprintf(stderr, "rsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, RSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += RSH_TOK_BUFSIZE;
            tokens_backup = tokens;
            tokens = (char **)realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                free(tokens_backup);
                fprintf(stderr, "rsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, RSH_TOK_DELIM);
    }

    tokens[position] = NULL;
    *argc = position;
    //printf("position = %d \n", position);

    return tokens;
}

int rsh_command(const char *cmd)
{
	char **argv;
	int argc;

	argv = rsh_split_line((char *)cmd, &argc);
	rsh_execute(argc, argv);

	return 0;
}

void rsh_loop(void)
{
    char *line;
    char **argv;
    int argc;
    int status;

	printf("Ricardo Shell (RSH)\n");

    do {
        printf("rsh> ");
        line = rsh_read_line();
        argv = rsh_split_line(line, &argc);
        status = rsh_execute(argc, argv);

        free(line);
        free(argv);
    } while (!status);
}
