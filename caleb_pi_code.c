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
//	char buf [100] = "Start";
	
	fd  = serialOpen("/dev/serial0", 9600);	
	usleep(2000000);
	serialFlush(fd);
	
	serialPuts(fd, "Start");

	fd2 = open("rail_voltages.dat", O_WRONLY);

	if(fd < 0){
		printf("cannot open \n");
		return -1;
	}

	if(fd2 < 0){
		printf("cannot open output");
	}
	
	while(1){
		input_len = serialDataAvail(fd);
		if(input_len > 41)
			input_len = 41;
		
		for(i = 0; i < input_len; i++){
			buffer[i] = serialGetchar(fd);
			printf("%c", buffer[i]);
		}
		printf("\n");
		write(fd2, buffer, input_len);
		//buffer[input_len] = '\0';
		//printf("%s\n", buffer);
		usleep(1000000);
	}	
	serialFlush(fd);

	close(fd2);
	close(fd);

	
}
