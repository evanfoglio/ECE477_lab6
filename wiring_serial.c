#include <wiringSerial.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int main()
{
	//serial descriptor
	int fd;
	int input_len;
	char buffer[41];
	int i;
	int fd2;
	
	//open serial port
	fd  = serialOpen("/dev/serial0", 9600);	
	//serial port error checking
	if(fd < 0){
		printf("Could not open serial port 0\n");
		return -1;
	}
	usleep(2000000);// let serial sort itself out
	
	//clear all awaiting data
	serialFlush(fd);
	
	//open outpur file
	fd2 = open("rail_voltages.dat", O_WRONLY);
	//output file error checking
	if(fd2 < 0){
		printf("cannot open output file \"rail_voltages.dat\"");
		return -1;
	}
	
	while(1){
		input_len = serialDataAvail(fd);
		if(input_len > 41)
			input_len = 41;
		//print input string
		for(i = 0; i < input_len; i++){
			buffer[i] = serialGetchar(fd);
			printf("%c", buffer[i]);
		}
	}	

	close(fd2);
	close(fd);

	
}
