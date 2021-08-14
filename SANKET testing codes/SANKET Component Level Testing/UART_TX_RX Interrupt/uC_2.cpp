#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
char a = 0;


int main(void){
	
	DDRB = 0xFF;
	sei();
	
	int ubrr = 51;
	UBRR0H = (char)(ubrr>>8);
	UBRR0L = (char) ubrr;
	
	//Initialize UART0
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);
    while (1) {
    }
}

ISR(USART0_RX_vect){
	a = UDR0;
	PORTB ^= 1;
	UDR0 = a;
	_delay_us(500);
	
}