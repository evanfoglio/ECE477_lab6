
int main(){

}
void USART_Init( unsigned int baud )
{
	UBRRn = 0;
	/* Setting the XCKn port pin as output, enables master
	mode. */
	XCKn_DDR |= (1<<XCKn);
	/* Set MSPI mode of operation and SPI data mode 0. */
	UCSRnC = (1<<UMSELn1)|(1<<UMSELn0)|(0<<UCPHAn)|(0<<UCPOLn);
	/* Enable receiver and transmitter. */
	UCSRnB = (1<<RXENn)|(1<<TXENn);
	/* Set baud rate. */
	/* IMPORTANT: The Baud Rate must be set after the
	transmitter is enabled */
	UBRRn = baud;
}

unsigned char USART_Receive( void )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRnA & (1<<UDREn)) );
	/* Put data into buffer, sends the data */
	UDRn = data;
	/* Wait for data to be received */
	while ( !(UCSRnA & (1<<RXCn)) );
	/* Get and return received data from buffer */
	return UDRn;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSRnA & (1<<RXCn)) );
	/* Get and return received data from buffer */
	 return UDRn;
}
