#include <avr/io.h>
#define F_CPU 8000000UL
# include <util/delay.h>

void Init(){
	DDRA = 0b11111111;											//Set I/O pins for PORT A
	int ubrr = 51;												//ubrr value
	UBRR0H = (char)(ubrr>>8);									//set ubrr value in registers
	UBRR0L = (char)(ubrr);
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
	ADMUX = 1<<ADLAR | (char)(pin)<<MUX0;
	ADCSRA |= 1<<ADSC;
	while(ADCSRA & 1<<ADSC)
	;
	value = ADCH;
	return value;												//return 8-bit digital value
}
int main(void){
	
	Init();														//Initialization
	char currSensor;											//variable of currentsensor value
    while (1){
		currSensor = ADCConversion(0);							//Converting input value
		USARTTransmit(currSensor);								//Transmitting converted current sensor value
		_delay_ms(10);
		
    }
}

