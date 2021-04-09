#include <wiringSerial.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	//serial descriptor
	int fd;
	int input_len;
	char buffer[100];
	int i;
	int fd2;

	fd  = serialOpen("/dev/serial0", 9600);	
	usleep(1000000);
	
	serialPrintf(fd, "START");

	fd2 = open("pi_output.txt", O_WRONLY);

	if(fd < 0){
		printf("cannot open \n");
		return -1;
	}

	if(fd2 < 0){
		printf("cannot open output");
	}
	
	while(1){
		input_len = serialDataAvail(fd2);
		if(input_len > 100)
			input_len = 100;
		
		for(i = 0; i < input_len; i++){
			buffer[i] = serialGetchar(fd);
		}

		write(fd2, buffer, input_len);
		
		buffer[input_len] = '\0';
		printf("%s", buffer);
	}	
	serialFlush(fd);

	close(fd2);
	close(fd);

	
}
