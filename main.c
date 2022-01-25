#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
    printf("Got %d args\n", argc);
    for(int i = 0; i < argc; i++) printf("arg[%d] = %s\n", i, argv[i]);
    return 0;
}
