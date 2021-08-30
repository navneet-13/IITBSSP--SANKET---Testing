#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#define F_CPU 8000000UL
#include <util/delay.h>

void Init(){
	DDRA = 0b00000000;											//Set I/O pins for PORT A
	DDRB = 0xff;
	int ubrr = 51;												//ubrr value
	UBRR0H = (char)(ubrr>>8);										//set ubrr value in registers
	UBRR0L = (char)(ubrr);
	UCSR0B = 1<<RXEN | 1<<TXEN;									//Enable receive and transmit for USART
	UCSR0C = 1<<UCSZ00 | 1<<UCSZ01 | 1<<USBS0;					//Select number of stop bit and bit size of data
}

char DeploymentStatus(){
	return ((PINA & 1<<0) | (PINA & 1<<1))<<6;					//check for detection switch status
}

void USARTTransmit(char data){
	while(!(UCSR0A & 1<<UDRE0))									//Transmit data using USART
		;
	UDR0  = data;
	_delay_ms(10);
	
}
int main(void){
	Init();														//INitializing
	
    while (1) {
		_delay_ms(100);											
		USARTTransmit(DeploymentStatus());						//Transmitting detection switch status
    }
}

