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

void USARTTransmit(char data){
	while(!(UCSR0A & 1<<UDRE0))									//Transmit data suing USART
	;
	UDR0  = data;
	_delay_ms(10);
	
}

int main(void){
	Init();
	
    while (1) {
	USARTTransmit(0x08);										//Transmit DATA
	PORTA = 0x08;												//Display sent data on PORT A
	_delay_ms(100);
	
	USARTTransmit(0x07);										//Transmit DATA
	PORTA = 0x07;												//Display sent data on PORT A
	_delay_ms(100);
    }
}

