#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>

void Init(){
	sei();
	DDRA = 0xff;												//Set I/O pins for PORT A
	DDRB = 0xff;												//Set I/O pins for PORT B
	int ubrr = 51;												//ubrr value
	UBRR0H = (char)(ubrr>>8);									//set ubrr value in registers
	UBRR0L = (char)(ubrr);
	UCSR0B = 1<<RXEN | 1<<TXEN | 1<<TXCIE0 | 1<<RXCIE0;			//Enable receive and transmit for USART
	UCSR0C = 1<<UCSZ00 | 1<<UCSZ01 | 1<<USBS0;					//Select number of stop bit and bit size of data
	
}

int main(void){
	Init();
	
    while (1) {
		UDR0 = 0b10100011;										//1st data transmission
		_delay_ms(100);
		UDR0 = 0b10001110;										//2nd data transmission
		_delay_ms(100);
    }
}

ISR(USART0_TX_vect){
	PORTB ^= 1;													//Toggle pinB0 after transmission
}

ISR(USART0_RX_vect){
	PORTA = UDR0;												//display received data on PORTA
}