#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
char receiveData = 0;

void Init(){
	sei();
	DDRB = 0xff;												//Set I/O pins for PORT B
	int ubrr = 51;												//ubrr value
	UBRR0H = (char)(ubrr>>8);									//set ubrr value in registers
	UBRR0L = (char)(ubrr);
	UCSR0B = 1<<RXEN | 1<<TXEN | 1<<RXCIE0;						//Enable receive and transmit for USART
	UCSR0C = 1<<UCSZ00 | 1<<UCSZ01 | 1<<USBS0;					//Select number of stop bit and bit size of data
	
}


int main(void){
	Init();
    while (1) {
		_delay_ms(100);
    }
}

ISR(USART0_RX_vect){
	receiveData = UDR0;
	PORTB ^= 1;													//Toggle pinB0 on receiving data
	UDR0 = receiveData;											//Send the receive data
	
}