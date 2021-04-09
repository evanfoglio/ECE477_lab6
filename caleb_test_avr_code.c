#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <math.h>
#include <avr/sleep.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

int serial_putchar(char, FILE *);
int serial_getchar(FILE *);
static FILE serial_stream = FDEV_SETUP_STREAM (serial_putchar, serial_getchar, _FDEV_SETUP_RW);

void init_serial(void);
void init_adc(void);
unsigned int read_adc(void);
unsigned char get_character(void);

void update_clock_speed(void);


int main()
{
  int i;	
  char buffer[100]="notStart";
  unsigned char buf[6];
  unsigned char start[6] = "Start";
  float estimate;
  char num_buf[8]; 
  unsigned char startbuf[6];
  update_clock_speed();  //adjust OSCCAL
  init_serial();
  init_adc();
  _delay_ms(1000); //let serial work itself out
//  while(strncmp("Start",startbuf,strlen("Start"))!=0) fgets(buffer,100,stdin);
	while(1){
		for(i = 0; i < 6; i++){
			buffer[i] = get_character();	
			printf("%c", buffer[i]);
			if(buffer[i] == start[i]){
				printf("hooray\n");
			} else {
				printf("No    \n");
			}
		}
	}
//	for(int i = 0; i < 6; i++){
//	buffer[i] = serial_getchar(&serial_stream);
//	}

  while(1) //raspberry pi controls reset line
  {
    estimate = ((1.1)/read_adc()) * 0x3FF;
    dtostrf(estimate, 1, 6, num_buf);
    printf("The power rail is approximately %s\n",num_buf);
  }    
}


//read the first two bytes of eeprom, if they have been programmed
//use the first byte as an offset for adjusting OSCCAL and the second as
//the direction to adjust 0=increase, 1=decrease.
//Any offset is allowed and users are cautioned that it is possible to
// adjust the oscillator beyond safe operating bounds.
void update_clock_speed(void)
{
  char temp;
  temp=eeprom_read_byte((void *)1); //read oscillator offset sign 
                                    //0 is positive 1 is  negative
                                    //erased reads as ff (so avoid that)
  if(temp==0||temp==1)      //if sign is invalid, don't change oscillator
  {
      if(temp==0)
          {
             temp=eeprom_read_byte((void *)0);
                 if(temp != 0xff) OSCCAL+=temp;
          }
          else
          {
             temp=eeprom_read_byte((void *)0);
                 if(temp!=0xff) OSCCAL -=temp;
          }
  }
 
}

/* Initializes AVR USART for 9600 baud (assuming 8MHz clock) */
/* 8MHz/(16*(51+1)) = 9615 about 0.2% error                  */
void init_serial(void)
{
   UBRR0H=0;
   UBRR0L= 51;
   UCSR0A=0;
   UCSR0C= (1<<USBS0)|(3<<UCSZ00) ;  // 8 BIT NO PARITY 2 STOP
   UCSR0B=(1<<RXEN0)|(1<<TXEN0)  ; //ENABLE TX AND RX ALSO 8 BIT
   stdin=&serial_stream;
   stdout=&serial_stream;

}   
//simplest possible putchar, waits until UDR is empty and puts character
int serial_putchar(char val, FILE * fp)
{
  while((UCSR0A&(1<<UDRE0)) == 0); //wait until empty 
   UDR0 = val;
   return 0;
}

//simplest possible getchar, waits until a char is available and reads it
//note:1) it is a blocking read (will wait forever for a char)
//note:2) if multiple characters come in and are not read, they will be lost
int serial_getchar(FILE * fp)
{
   while(!(UCSR0A&(1<<RXC0)));  //WAIT FOR CHAR
   return UDR0;
}

unsigned char get_character()
{
   while(!(UCSR0A&(1<<RXC0)));  //WAIT FOR CHAR
   return UDR0;
}

void init_adc(void)
{
	ADMUX = (3<<REFS0) | 8; //Refference voltage is 1.1V
	//8MHz / 64 = 125kHz
	ADCSRA = (1<<ADEN) | (6<<ADPS0); // enable ADC, prescaler=64	
	ADCSRB = 0;
	DIDR0 = 0;
}
unsigned int read_adc(void)
{
    ADCSRA |= (1<<ADSC);
    while(ADCSRA & (1<<ADSC)); //wait for coversion
    return ADC;
}
