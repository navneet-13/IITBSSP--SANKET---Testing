#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

int main(void){
	DDRB = 0xFF;
	_delay_ms(10);
	PORTB = 1<<PINB0;
    while (1){
    }
}

