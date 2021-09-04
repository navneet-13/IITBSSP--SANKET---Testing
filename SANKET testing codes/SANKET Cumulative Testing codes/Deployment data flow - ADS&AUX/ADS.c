#include <avr/io.h>
#define F_CPU 800000UL
#include <util/delay.h>

void Init(){
	DDRA = 0b00001001;											//Set I/O pins for PORT A
	DDRB;
	int ubrr = 51;												//ubrr value
	UBRR0H = (char)(ubrr>>8);									//set ubrr value in registers
	UBRR0L = (char)(ubrr);
	UCSR0B = 1<<RXEN | 1<<TXEN;									//Enable receive and transmit for USART
	UCSR0C = 1<<UCSZ00 | 1<<UCSZ01 | 1<<USBS0;					//Select number of stop bit and bit size of data
	
	TWAR = 0b10101100;											//Slave adress
	ADCSRA = 1<<ADEN | 1<<ADPS2;								//Enable ADC and set Prescaler
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

void checkStatusCode(char data){
	while(!(TWCR & 1<<TWINT))									//Twint Flag set 
		;
	while(!(TWSR == data))										//Check status code
		;
}

void CurrentLimiter(char command){								//Turn current limiter on/off
	if (command == 1){											//command - takes value 0: turn off, 1: turn on
		PORTB = 1<<PINB0;
	}
	
	else if(command == 0){
		PORTB = 0<<PINB0;
	}
}

char CheckSwitchStatus(){
	char status;
	status = ((PINA & 1<<PINA4) | (PINA & 1<<PINA5))>>4;		//check switch status connected to PINA4 and PINA5
	return status;
}

void checkAndTransmit(char code, char data){
	while(!(TWCR & 1<<TWINT))									//Twint Flag set
		;
	while(!(TWSR == code))										//Check for status code
		;
	TWDR = data;												//Write data to be transmitted in TWDR
}

int main(void){
	
	Init();										
	
	char currSensor1, currSensor2;
	char switchStatus;
    
	while (1){
		
		TWCR = 1<<TWEN | 1<<TWINT | 1<<TWEA;					//Waiting for command from master
		checkStatusCode(0x60);
		
		TWCR = 0<<TWEA | 1<<TWINT | 1<<TWEN;					
		checkStatusCode(0x88);									//Check for status code whether data is received and nack is sent
		
		TWCR = 1<<TWINT | 1<<TWEA | 1<<TWEN;					//Unaddressed Slave
		_delay_ms(5);
		
		
		CurrentLimiter(1);										//Enable current limiter for Burner Circuit
		_delay_ms(10);											
		
			
		currSensor1 = ADCConversion(0);							//ADC conversion for 1st current sensor
		currSensor2 = ADCConversion(1);							//ADC conversion for 2nd current sensor
		switchStatus = CheckSwitchStatus();
		
		
		PORTA |= 1<<PINA3;										//Tell AUX that HM data is ready to be sent
		
		TWCR = 1<<TWINT | 1<<TWEA | 1<<TWEN;
		checkAndTransmit(0xA8, switchStatus);					//Deployment switch state sent											
		TWCR  = 1<<TWEN | 1<<TWINT | 1<<TWEA;	
		
		checkAndTransmit(0xB8, currSensor1);					//First Current Sensor Value sent											
		TWCR = 1<<TWINT | 1<<TWEA | 1<<TWEN;

		checkAndTransmit(0xB8, currSensor2);					//Second Current Sensor Value Sent
		TWCR = 1<<TWINT | 0<<TWEA | 1<<TWEN;
		
		checkStatusCode(0xC0);
		
		TWCR = 1<<TWINT | 1<<TWEA | 1<<TWEN;					//Unaddressed Slave
		PORTA ^= 1<<PINA3;
    }
}

