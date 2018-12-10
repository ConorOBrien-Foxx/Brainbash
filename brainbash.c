#include <stdio.h>
#include <stdlib.h>

#include "brainbash.h"

brainbash* brainbash_init(char* program, size_t size, bool use_color) {
    brainbash* res = malloc(sizeof(brainbash));
    
    if(res == NULL)
        return res;
    
    res->program = program;
    res->size = size;
    res->focus = 0;
    res->index = 0;
    res->use_color = use_color;
    
    // size_t depth_size = 1;
    // for(size_t p = 0; p < size; p++) {
        // if(program[p] == '[')
            // depth_size++;
    // }
    // res->depths = calloc(depth_size, sizeof(int));
    // res->depth_pointer = 0;
    
    for(int i = 0; i < NUM_OF_TAPES; i++) {
        res->tapes[i] = malloc(sizeof(tape));
        if(res->tapes[i] == NULL) {
            for(int j = 0; j < i; j++) {
                free(res->tapes[j]);
            }
            free(res);
            return NULL;
        }
        res->tapes[i]->ptr = 0;
        res->tapes[i]->farthest = 0;
        for(int j = 0; j < MAX_SIZE; j++) {
            res->tapes[i]->data[j].value = 0;
            res->tapes[i]->data[j].max   = 256;
        }
    }
    
    return res;
}

int64_t tape_get(tape* t) {
    // printf("focus = %i\nptr = %i", inst->focus, inst->ptrs[inst->focus]);
    // tape* t = inst->tapes[inst->focus];
    return t->data[t->ptr].value;
}
int64_t tape_get_at(tape* t, int pos) {
    // printf("focus = %i\nptr = %i", inst->focus, inst->ptrs[inst->focus]);
    // tape* t = inst->tapes[inst->focus];
    return t->data[pos].value;
}
void tape_set(tape* t, int64_t value) {
    // tape* t = inst->tapes[inst->focus];
    cell* k = &t->data[t->ptr];
    while(value < 0)
        value += k->max;
    k->value = value % k->max;
    // cell* k = &inst->tapes[inst->focus]->data[inst->ptrs[inst->focus]];
    // k->value = value % k->max;
}
void tape_set_ptr(tape* t, int new_ptr) {
    t->ptr = new_ptr;
    if(new_ptr > t->farthest)
        t->farthest = new_ptr;
}

void cls(void) {
    printf("\x1b[2J");
}

void tape_display(tape* t, bool use_color) {
    for(int j = 0; j <= t->farthest; j++) {
        if(use_color) {
            printf(" %s%-*lli\x1b[0m ", j == t->ptr ? "\x1b[33m>" : " ", 4, tape_get_at(t, j));
        }
        else {
            printf(" %s%-*lli ", j == t->ptr ? ">" : " ", 4, tape_get_at(t, j));
        }
    }
}

void brainbash_display(brainbash* inst, bool use_color) {
    for(int i = 0; i < NUM_OF_TAPES; i++) {
        tape* cur = inst->tapes[i];
        printf("[%c] ", i == inst->focus ? '*' : ' ');
        tape_display(cur, use_color);
        puts("");
    }
}

void brainbash_next_focus(brainbash* inst) {
    inst->focus++;
    inst->focus %= NUM_OF_TAPES;
}

int brainbash_step(brainbash* inst) {
    if(inst->index >= inst->size)
        return 0;
    char cur = inst->program[inst->index];
    tape* cur_tape = inst->tapes[inst->focus];
    if(cur == '[') {
        if(!tape_get(cur_tape)) {
            int depth = 1;
            inst->index++;
            while(depth && inst->index < inst->size) {
                // putchar(inst->program[inst->index]);
                if(inst->program[inst->index] == '[')
                    depth++;
                if(inst->program[inst->index] == ']')
                    depth--;
                inst->index++;
            }
        }
        else inst->index++;
    } else if(cur == ']') {
        if(tape_get(cur_tape)) {
            int depth = 1;
            inst->index--;
            while(depth && inst->index < inst->size) {
                if(inst->program[inst->index] == ']')
                    depth++;
                if(inst->program[inst->index] == '[')
                    depth--;
                inst->index--;
            }
            inst->index++;
        }
        else inst->index++;
    } else if(cur == '{') {
        if(!tape_get(cur_tape)) {
            int depth = 1;
            inst->index++;
            // printf("`%c`\n",inst->program[inst->index]);
            while(depth && inst->index < inst->size) {
                // putchar(inst->program[inst->index]);
                if(inst->program[inst->index] == '{')
                    depth++;
                if(inst->program[inst->index] == '}')
                    depth--;
                inst->index++;
            }
        }
        else inst->index++;
    } else if(cur == '}') {
        inst->index++;
    } else {
        brainbash_exec_instruction(inst, cur);
        inst->index++;
    }
    
    return 1;
}

void brainbash_run(brainbash* inst) {
    while(brainbash_step(inst));
}

void brainbash_debug(brainbash* inst, bool color) {
    // int k = 0;
    while(1) {
        // printf("%i", k);
        // if(k) {
            // k--;
        // } else {
            cls();
            puts(inst->program);
            printf("%*c\n", 1 + (int)inst->index, '^');
            brainbash_display(inst, color);
            // scanf("%d",&k);
            getchar();
        // }
        if(!brainbash_step(inst))
            break;
    }
    puts("Done");
    getchar();
}

void brainbash_exec_instruction(brainbash* inst, char instruction) {
    // printf("`%c`", instruction);
    tape* cur = inst->tapes[inst->focus];
    switch(instruction) {
        case '+':
            tape_set(cur, tape_get(cur) + 1);
            break;
        case '-':
            tape_set(cur, tape_get(cur) - 1);
            break;
        case '>':
            tape_set_ptr(cur, cur->ptr + 1);
            break;
        case '<':
            tape_set_ptr(cur, cur->ptr - 1);
            break;
        case '.':
            putchar(tape_get(cur));
            break;
        case ',':
            tape_set(cur, getchar());
            break;
        // switch focus
        case '~':
            brainbash_next_focus(inst);
            break;
        // switch focus, copying position
        case '*':
            brainbash_next_focus(inst);
            tape_set_ptr(inst->tapes[inst->focus], cur->ptr);
            break;
        // output number
        case ';':
            printf("%lli", tape_get(cur));
            break;
        // debug
        case '`':
            brainbash_display(inst, inst->use_color);
            break;
        // input number
        case '#':
            {
                int temp, result;
                result = scanf("%d", &temp);
                if(result == EOF) {
                    tape_set(cur, -1);
                }
                
                else if(result == 0) {
                    tape_set(cur, -1);
                }
                
                else {
                    tape_set(cur, temp);
                }
            }
            break;
    }
}

// void execute_brainbash(brainbash* inst, char* str, size_t size) {
    
// }

// void interpret_brainbash(char* str, size_t size) {
    // brainbash* inst = brainbash_init();
    
    // execute_brainbash(inst, str, size);
// }