#include <wiringSerial.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
int init(void);
int main()
{
	//serial descriptor
	int fd;
	char buffer[42];
	FILE *fd2 = NULL;
	FILE *serial_in;
	int fdserial = init();	
	//open serial port
	fd = serialOpen("/dev/serial0", 9600);	
	//serial port error checking
	if(fd < 0){
		printf("Could not open serial port 0\n");
		return -1;
	}

	//fd2 = fopen("rail_voltages.dat","w");

	//if(fd2==NULL){
	//	printf("Error opening output file\n");
	//	return -1;
	//}
	serial_in = fdopen(fdserial, "r");
	
	usleep(2000000);// let serial sort itself out
	
	//clear all awaiting data
	serialFlush(fd);
	

	while(fgets(buffer, 42, serial_in)){
		printf("%s", buffer);
		
		fd2 = fopen("rail_voltages.dat","a");

	        if(fd2==NULL){
        	        printf("Error opening output file\n");
        	        return -1;
        	}

		fprintf(fd2, "%s", buffer);

		fclose(fd2);


	}
	close(fd);
}



int init()
{
	int fd1;
	struct termios tc; // terminal control structure

	//todo serial port should not be hard coded
	fd1 = open("/dev/serial0", O_RDWR | O_NOCTTY);
	if (fd1 < 1) {
		printf("Failed to open serial port\n");
		return 0;
	}
	tcgetattr(fd1, &tc);
	tc.c_iflag = IGNPAR;
	tc.c_oflag = 0;
	tc.c_cflag = CS8 | CREAD | CLOCAL; //8 bit chars enable receiver no modem status lines
	tc.c_lflag = ICANON;

	//todo baud rate should not be hard coded
	cfsetispeed(&tc, B9600);
	cfsetospeed(&tc, B9600);
	//todo should have bits per character set
	tcsetattr(fd1, TCSANOW, &tc);
	return fd1;
}
