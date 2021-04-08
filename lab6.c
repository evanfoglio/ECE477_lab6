#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int init(void);

int main(int argc, char *argv[])
{
	FILE *serial_out;
	FILE *serial_in;
	FILE *disk_out = NULL;
	int fdserial;
	char buffer[100];

	fdserial = init();
	if (fdserial < 1)
		exit(0);
	serial_out = fdopen(fdserial, "w");
	serial_in = fdopen(fdserial, "r");
	if (serial_out == NULL || serial_in == NULL)
		printf("fdopen failed \n"), exit(0);

	disk_out = fopen("output_file", "w");
	if (disk_out == NULL)
		disk_out = stdout,
		printf("couldn't open %s using stdout\n", argv[1]);

	fprintf(serial_out, "Start\n");
	while (fgets(buffer, 100, serial_in))
		fputs(buffer, disk_out);
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
	tc.c_cflag = CS8 | CREAD |
		     CLOCAL; //8 bit chars enable receiver no modem status lines
	tc.c_lflag = ICANON;

	//todo baud rate should not be hard coded
	cfsetispeed(&tc, B9600);
	cfsetospeed(&tc, B9600);
	//todo should have bits per character set
	tcsetattr(fd1, TCSANOW, &tc);
	return fd1;
}
