#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#define TAPESIZE 1024
#define HALT	-1

static const int debug = 1;

struct tuple {
	int new_state;
	char new_sym;
	char new_dir;
};

struct TM {
	struct tuple *instrcts;
	int n_tuples;
	char *syms;		// is null-terminated
	int n_syms;
	int *sym_map;		// map symbol -> index in syms
	char *tape;
	int loc;		// location on tape
	int cur_state;
	int n_states;
};

struct TM *tm_new(const char *symbols,
		  const int *new_states,
		  const char *new_syms,
		  const char *new_dirs,
		  int n_states, int n_syms);

void tm_init(struct TM *tm, int init_state, int loc, const char *tape);

int tm_step(struct TM *tm);

#define tm_run(tm) while(tm_step(tm) != HALT)

void tm_destroy(struct TM *tm);

int main(int argc, char *argv[])
{
	struct TM *tm;
	char *symbols;
	int n_states, n_syms;
	int *new_states;
	char *new_syms, *new_dirs;
	int n_tuples;
	char dir;
	FILE *program;
	char *tape_buffer;
	size_t tape_size = 1024;
	ssize_t tape_read;

	if (argc < 2) {
		fprintf(stderr, "usage: %s [program.tur]\n", argv[0]);
		return 1;
	} else if ((program = fopen(argv[1], "r")) == NULL) {
		const char *errstr = strerror(errno);
		fprintf(stderr, "%s: %s\n", argv[1], errstr);
		return 1;
	}
	
	fscanf(program, "%d %d", &n_states, &n_syms);
	n_tuples = n_states * n_syms;
	symbols = malloc(n_syms+1);
	symbols[n_syms] = 0;
	fscanf(program, "%s", symbols);

	if (debug)
		printf("states: %d, symbols(%d): %s\n",
		       n_states, n_syms, symbols);
	new_states = malloc(sizeof(*new_states) * n_tuples);
	new_syms = malloc(n_tuples);
	new_dirs = malloc(n_tuples);
	for (int i=0; i<n_tuples; ++i) {
		fscanf(program, "%d %c %c", new_states+i, new_syms+i, &dir);
		new_dirs[i] = dir == '+' ? 1 : -1;
		if (debug)
			printf("(%d %c) -> (%d %c %c)\n",
			       i/n_syms, symbols[i%n_syms],
			       new_states[i], new_syms[i], dir);
	}

	tm = tm_new(symbols, new_states, new_syms, new_dirs, n_states, n_syms);
	
	// read tape from standard in
	tape_buffer = malloc(tape_size);
	tape_read = getline(&tape_buffer, &tape_size, stdin);
	if (tape_read < 1 || tape_buffer[0] == '\n')
		fprintf(stderr, "no tape given\n");
	else {
		tape_buffer[tape_read-1] = 0;
		tm_init(tm, 0, 0, tape_buffer);
		tm_run(tm);
	}

	fclose(program);
	free(new_states);
	free(new_syms);
	free(new_dirs);

	return 0;
}

struct TM *tm_new(const char *symbols,
		  const int *new_states,
		  const char *new_syms,
		  const char *new_dirs,
		  int n_states, int n_syms)
{
	struct TM *tm;

	tm = malloc(sizeof(*tm));
	tm->n_tuples = n_states * n_syms;

	tm->syms = strdup(symbols);
	tm->n_syms = n_syms;
	tm->sym_map = malloc(sizeof(*tm->sym_map) * 256);

	tm->n_states = n_states;
	tm->instrcts = malloc(sizeof(*tm->instrcts) * (tm->n_tuples + n_syms));
	for (int i=0; i<tm->n_tuples; ++i) {
		tm->instrcts[i].new_state = new_states[i];
		tm->instrcts[i].new_sym = new_syms[i];
		tm->instrcts[i].new_dir = new_dirs[i];
	}
	for (char *sym_i = tm->syms; *sym_i; ++sym_i)
		tm->sym_map[*sym_i] = sym_i - tm->syms;

	tm->tape = NULL;
	tm->cur_state = -1;
	tm->loc = 0;
	
	return tm;
}

void tm_init(struct TM *tm, int init_state, int loc, const char *tape)
{
	tm->cur_state = init_state;
	tm->loc = loc;
	tm->tape = strdup(tape);
}

int tm_step(struct TM *tm)
{
	if (tm->cur_state == HALT)
		return HALT;
	char sym = tm->tape[tm->loc];
	int p = tm->cur_state * tm->n_syms + tm->sym_map[sym];
	struct tuple next = tm->instrcts[p];

	if (debug) {
		printf("   %*sv\n", tm->loc, "");
		printf("1. %s\n", tm->tape);
	}

	if (tm->tape[tm->loc] == '\0') {
		fprintf(stderr, "%s: accessing memory out of tape bounds\n", __func__);
		abort();
	}
	tm->tape[tm->loc] = next.new_sym;
	tm->loc += next.new_dir;
	if (debug)
		printf("2. %s\n", tm->tape);
	return tm->cur_state = next.new_state;
}

void tm_destroy(struct TM *tm)
{
	free(tm->tape);
	free(tm->syms);
	free(tm->instrcts);
	free(tm->sym_map);
	free(tm);
}
