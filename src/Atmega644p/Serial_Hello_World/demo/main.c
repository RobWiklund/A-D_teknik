#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 20000000UL   // Set the CPU frequency
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1 // Asynchronous Normal mode s. 227

void UART_Init( unsigned int ubrr)
{
	// Set baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	//Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// Set frame format: 1stop bit, 8data
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}


void uart_send_char(unsigned char data) {
	while (!(UCSR0A & (1<<UDRE0)));     // Wait for the transmit buffer to be empty
	UDR0 = data;                        // Send the data
}

void uart_send_string(char *str) {
	while (*str) {
		uart_send_char(*str);           // Send each character of the string
		str++;
	}
}

int main(void) {
	UART_Init(MYUBRR); // initialize
	while (1) {
		uart_send_string("Helloworld\n");   // Send a message over UART		
		_delay_ms(1000);					// Wait for 1 second
	}
	return 0;
}
