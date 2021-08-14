#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>


int main(void){
	DDRB = 0xFF;
	DDRA = 0xFF;
	sei();
	
	int ubrr = 51;
	UBRR0H = (char)(ubrr>>8);
	UBRR0L = (char) ubrr;
	
	//Initialize UART0
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<TXCIE0) | (1<<RXCIE0);
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);

    while (1) {
		UDR0 = 0b10100011;
		_delay_ms(100);
		UDR0 = 0b10001100;
		_delay_ms(100);
    }
}

ISR(USART0_TX_vect){
	PORTB ^= 1;
}

ISR(USART0_RX_vect){
	PORTA = UDR0;
}