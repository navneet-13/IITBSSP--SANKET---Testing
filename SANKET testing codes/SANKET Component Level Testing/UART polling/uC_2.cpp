#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

int main(void){
	DDRA = 0xff;
	int ubrr = 51;
	UBRR0H =  (unsigned char) (ubrr>>8);
	UBRR0L =  (unsigned char) ubrr;
	
	
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);
	
	while (1) {
		
		while(!(UCSR0A & (1<<RXC)))
			;
		PORTA = UDR0;
	}
}
