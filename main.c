#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "brainbash.h"

int main(int argc, char** argv) {
    if(argc < 2) {
        fprintf(stderr, "Error: insufficient arguments given.\n");
        fprintf(stderr, "Usage: %s [flags] [file name]\n", argv[0]);
        return 1;
    }
    
    bool use_color = false;
    char* file_name = NULL;
    
    for(int i = 1; i < argc; i++) {
        if(argv[i][0] == '-') {
            int j = 0;
            while(argv[i][j]) {
                switch(argv[i][j]) {
                    case 'c':
                        use_color = true;
                        break;
                    
                    case '-':
                        break;
                    
                    default:
                        fprintf(stderr, "Unknown flag option `%c`\n", argv[i][j]);
                        break;
                }
                j++;
            }
        }
        else if(file_name == NULL) {
            file_name = argv[i];
        }
    }
    
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
    
    brainbash* inst = brainbash_init(contents, file_size, use_color);
    brainbash_run(inst);
    // brainbash_debug(inst);
    // brainbash_display(inst);
}