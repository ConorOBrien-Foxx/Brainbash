#ifndef BRAINBASH_INCL
#define BRAINBASH_INCL

#include <stdint.h>

#define MAX_SIZE (30000)
#ifndef NUM_OF_TAPES
#define NUM_OF_TAPES (2) /*always 2*/
#endif

typedef struct cell {
    int64_t value;
    int64_t max;
} cell;

typedef struct tape {
    cell data[MAX_SIZE];
    int ptr;
    int farthest;
} tape;

typedef struct brainbash {
    tape* tapes[NUM_OF_TAPES];
    int focus;
    size_t size;
    size_t index;
    // int* depths;
    // int depth_pointer;
    char* program;
} brainbash;

brainbash* brainbash_init(char*, size_t);
void brainbash_display(brainbash*);
void brainbash_run(brainbash*);
void brainbash_debug(brainbash*);
int brainbash_step(brainbash*);

void brainbash_exec_instruction(brainbash*, char);

// void execute_brainbash(brainbash* inst, char*, size_t);
// void interpret_brainbash(char*, size_t);

#endif