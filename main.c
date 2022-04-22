#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

void config_port(struct termios *tty){
    tty->c_cflag &= ~(PARENB | CSTOPB | CSIZE | CRTSCTS);
    tty->c_cflag |= CS8 | CREAD | CLOCAL;
    tty->c_lflag &= ~(ICANON | ECHO | ECHONL | ECHOE | ISIG);
    tty->c_iflag &= ~(IXON | IXOFF | IXANY | IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
    tty->c_oflag &= ~(OPOST | ONLCR);
    tty->c_cc[VTIME] = 20;
    tty->c_cc[VMIN] = 0;
    cfsetispeed(tty, B4800);
    cfsetospeed(tty, B4800);
}

//Call - at89prog <serial device> <hex file>
//For now only device read is supported...
int32_t main(int32_t argc, uint8_t **argv){
    if(argc != 3){
        fprintf(stderr, "Usage - at89prog <serial device> <hex file>\n");
        return 1;
    }
    uint8_t *ser_dev = argv[1];
    uint8_t *hexfile_path = argv[2];
     
    FILE *hexfile = fopen(hexfile_path, "w+");
        if(!hexfile){
            fprintf(stderr, "Error: %s\n",strerror(errno));
            return 2;
        }

    int16_t serial_port_fd  = open(ser_dev, O_RDWR);
    if(serial_port_fd < 0){
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return 3;
    }

    struct termios tty;

    if(tcgetattr(serial_port_fd, &tty)){
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return 4;
    }
    
    config_port(&tty);

    if(tcsetattr(serial_port_fd, TCSANOW, &tty)){
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return 5;
    }

    uint8_t msg[4] = {'R', 0x00, 0x00, 16};
    write(serial_port_fd, msg, sizeof(msg));

    usleep(50000);

    uint8_t rcv[16];
    memset(&rcv, '\0', sizeof(rcv));
    int32_t numread = read(serial_port_fd, &rcv, sizeof(rcv));
    if(numread < 0){
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return 6;
    }
    for(uint8_t *s = rcv; s && (s-rcv)<sizeof(rcv); s++)
        printf("%02x ", *s);
    printf("\n");
    
    close(serial_port_fd);
    //printf("Cool, working good!\n");

    //fprintf(hexfile, "test1");
    //fclose(hexfile);

    return 0;
}
