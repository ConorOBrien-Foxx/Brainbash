#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "brainbash.h"

int main(int argc, char** argv) {
    if(argc < 2) {
        fprintf(stderr, "Error: insufficient arguments given.\n");
        fprintf(stderr, "Usage: %s [file name]\n", argv[0]);
        return 1;
    }
    
    char* file_name = argv[1];
    FILE* file = fopen(file_name, "r");
    
    if(fseeko(file, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error reading file: %s\n", strerror(errno));
        return errno;
    }
    
    off_t file_size = ftello(file);
    
    if(fseeko(file, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Error reading file: %s\n", strerror(errno));
        return errno;
    }
    
    // + 1 for trailing null byte
    char* contents = malloc(sizeof(char) * (file_size + 1));
    fread(contents, file_size, sizeof(char), file);
    
    brainbash* inst = brainbash_init(contents, file_size);
    brainbash_run(inst);
    // brainbash_debug(inst);
    // brainbash_display(inst);
}