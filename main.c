#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
//#include <unistd.h>

//Call - at89prog <serial device> <hex file>
//For now only device read is supported...
int main(int argc, char **argv){
    if(argc != 3){
        fprintf(stderr, "Usage - at89prog <serial device> <hex file>\n");
        return 1;
    }
    uint8_t *ser_dev = argv[1];
    uint8_t *hexfile_path = argv[2];
     
    FILE *hexfile = fopen(hexfile_path, "w+");
        if(!hexfile){
            fprintf(stderr, "Error: %s\n",strerror(errno));
            return 1;
        }
    fprintf(hexfile, "test1");
    fclose(hexfile);

    return 0;
}
