#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>


int main(void){
	DDRA = 0b11111100;
	DDRB = 0xFF;
	int ubrr = 51;
	UBRR0H = (char)(ubrr>>8);
	UBRR0L = (char)(ubrr);
	UCSR0B = 1<<RXEN0 | 1<<TXEN0;
	UCSR0C = 1<<USBS0 | 1<<UCSZ01 | 1<<UCSZ00;
	UDR0 = 0b11;
	
    while (1) {
		_delay_ms(500);
		
		if(PINA == 0){
			UDR0 = 00;
			PORTB = 00;
		}
		
		else if(PINA == 0b00000001){
			UDR0 = 0b01;
			PORTB = 0b01;
		}
		
		
		else if(PINA == 0b000000010){
			UDR0 = 0b10;
			PORTB = 0b10;
		}
		
		else if(PINA == 0b00000011){
			UDR0 = 0b11;
			PORTB = 0b11;
		}
    }
}

