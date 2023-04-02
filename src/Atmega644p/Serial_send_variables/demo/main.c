#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#define F_CPU 20000000UL   // Set the CPU frequency
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1 // Asynchronous Normal mode s. 227

void UART_Init( unsigned int ubrr)
{
	// Set baud rate 9600
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
	
	int alarm = 0;			// 0 = No alarm | 1 = Alarm
	int mode = 1;			// 1 = Charging | 2 = Discharging
	int status = 1;			// 1 = ON | 2 = OFF | 3 = Disrupted | 4 = Finished
	int battery = 3200;		// Total capacity
	double c_rate = 0.5;			// Charging C Rate
	double capacity = 3012.211123;	// Battery capacity
	double temperature = 54.13331;	// Battery temperature
	double voltage = 3.98972;		// Battery Voltage
	double current = 1931.15911;		// Charging, Discharging current
	
	// Store stringified variables
	char str_arr[9][30];	// 9 variables in list, 30 positions for each item
	char str_output[100];	// Combined output of string
	

	UART_Init(MYUBRR); // initialize

	while (1) {
		// INT to string
		itoa(alarm, str_arr[0], 10); // itoa(int_value, to_location, data_format(decimal=10))
		itoa(mode, str_arr[1], 10);
		itoa(status, str_arr[2], 10);
		itoa(battery, str_arr[3], 10);
		// Double to string
		dtostrf( c_rate, 1, 2, str_arr[4]); // dtostrf(float_value, min_width, num_after_decimal, to_location)
		dtostrf( capacity, 1, 2, str_arr[5]);
		dtostrf( temperature, 1, 2, str_arr[6]);
		dtostrf( voltage, 1, 2, str_arr[7]);
		dtostrf( current, 1, 2, str_arr[8]);
		
		// ------------------- COMBINE STRINGS ------------------- //
		
		int i; // For-loop counter
		int size = sizeof str_arr / sizeof *str_arr; // amount of items in array
		
		for (i = 0; i <= size - 1; ++i)
		{
			strcat(str_output, str_arr[i]); // Add item to string output each time
			if (i != size - 1) {			// not on last row
			strcat(str_output, ",");		// CSV
			}
		}	strcat(str_output, "\n");		// New line after string output has been merged/combined
		
		// ------------------- SEND AND CLEAR STRING ------------------- //
		
		uart_send_string(str_output);	// Send a message over UART		
		_delay_ms(90000);				// Not accurate delay, ~ 1-2s
		memset(str_output,0,sizeof(str_output));	// Clear string output
	}
	return 0;
}
