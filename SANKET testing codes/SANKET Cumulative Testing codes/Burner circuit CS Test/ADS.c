#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>


void Init(){
	DDRA = 0xff;												//Set I/O pins for PORT A
	int ubrr = 51;												//ubrr value
	UBRR0H = char(ubrr>>8);										//set ubrr value in registers
	UBRR0L = char(ubrr);
	UCSR0B = 1<<RXEN | 1<<TXEN;									//Enable receive and transmit for USART
	UCSR0C = 1<<UCSZ00 | 1<<UCSZ01 | 1<<USBS0;					//Select number of stop bit and bit size of data
	
	ADCSRA = 1<<ADEN | 1<<ADPS2;								//Enable ADC and set Prescaler
}

void USARTTransmit(char data){
	while(!(UCSR0A & 1<<UDRE0))									//Transmit data suing USART
	;
	UDR0  = data;
	_delay_ms(10);
	
}

char ADCConversion(int pin){									// start ADC conversion
	char value;													// pin - this variable decides which analogue pin will be used for conversion
	ADMUX = 1<<ADLAR | char(pin)<<MUX0;
	ADCSRA |= 1<<ADSC;
	while(ADCSRA & 1<<ADSC)
	;
	value = ADCH;
	return value;												//return 8-bit digital value
}

void CurrentLimiter(char command){								//Turn current limiter on/off
	if (command == 1){											//command - takes value 0: turn off, 1: turn on
		PORTA = 1<<PINA0;
	}
	
	else if(command == 0){
		PORTA = 0<<PINA0;
	}
}

int main(void){
	
    Init();
	char adcValue;
	
	while (1) {
		CurrentLimiter(1);
		
		adcValue = ADCConversion(0);
		USARTTransmit(adcValue);
		_delay_ms(20);
    }
}

