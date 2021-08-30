#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

void Init(){
	DDRA = 0xff;											//Set I/O pins for PORT A
	int ubrr = 51;												//ubrr value
	UBRR0H = (char)(ubrr>>8);										//set ubrr value in registers
	UBRR0L = (char)(ubrr);
	UCSR0B = 1<<RXEN | 1<<TXEN;									//Enable receive and transmit for USART
	UCSR0C = 1<<UCSZ00 | 1<<UCSZ01 | 1<<USBS0;					//Select number of stop bit and bit size of data
}

char USARTReceive(){
	while(!(UCSR0A & (1<<RXC)))
		;															//Receive data suing USART
	return UDR0;

}

int main(void){
	Init();
	
	while (1) {
		PORTA = USARTReceive();										//Receive data
		
	}
}
