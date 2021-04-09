#include <wiringSerial.h>
#include <stdio.h>
#include <unistd.h>

int main()
{

	usleep(1000000);
	//serial descriptor
	int fd;
	int input_len;
	char buffer[100];
	int i;
	fd  = serialOpen("/dev/serial0", 9600);
	if(fd < 0){
		printf("cannot open \n");
		return -1;
	}

	input_len = serialDataAvail(fd);

	if(input_len > 100)
		input_len = 100;
	for(i = 0; i<input_len; i++){
		buffer[i] = serialGetchar(fd);
		printf("%c", buffer[i]);
	}
	serialFlush(fd);

	close(fd);

	
}
