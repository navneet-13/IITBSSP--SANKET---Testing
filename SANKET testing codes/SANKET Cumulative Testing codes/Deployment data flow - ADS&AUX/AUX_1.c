#include <avr/io.h>
#define F_CPU 800000UL
#include <util/delay.h>

void Init(){
	DDRA = 0x00000001;												//SET I/O Pins for PORT A<B
	DDRB;
	int ubrr = 51;													//define ubrr
	UBRR0H = (char)ubrr>>8;											//Set ubrr in registers
	UBRR0L = (char)ubrr;
	UCSR0B = 1<<RXEN0 | 1<<TXEN0;	
	UCSR0C = 1<<USBS0 | 1<<UCSZ01 | 1<<UCSZ00;
}

void checkStatusCode(char data){
	while(!(TWCR & 1<<TWINT))										//Twint Flag set 
	;
	while(!(TWSR == data))											//Check status code
	;
}

void I2CTransmit(char data){
	while(!(TWCR & 1<<TWINT))										//Twint Flag set
		;
	TWDR = data;													//Write data to be transmitted in TWDR
}

void CurrentLimiter(char command){									//Turn current limiter on/off
	if (command == 1){												//command - takes value 0: turn off, 1: turn on
		PORTA = 1<<PINA0;
	}
	
	else if(command == 0){
		PORTA = 0<<PINA0;
	}
}
	
int main(void){
	
	Init();															//Initialize
	char slaveAddress  = 0b10101100;								//Slave Address
	char deploymentCommand = 0b11001100;							//deployment message
	char depolymentStatus;											
	char currentSenosor_1, currentSenosor_2;						//Variable for current sensor value
    while (1){
		
		CurrentLimiter(1);
		_delay_ms(100);
		
		TWCR = 1<<TWEN | 1<<TWINT | 1<<TWSTA;						//START
		
		checkStatusCode(0x08);										//ADDRESS of Slave
		I2CTransmit(slaveAddress | 0);
		TWCR = 1<<TWINT | 1<<TWEN;
		
		checkStatusCode(0x18);
		I2CTransmit(deploymentCommand);								//Deployment Command
		TWCR = 1<<TWEN | 1<<TWINT;
		
		checkStatusCode(0x30);
				
		TWCR = 1<<TWEN | 1<<TWINT | 1<<TWSTO;						//Stop command sent
		
		while(!(PINA & 1<<PINA1))									//Checking for confirmation from ADS to Start TWI Communication
			;
		
		TWCR = 1<<TWSTA | 1<<TWEN | 1<<TWINT;						//Start
		checkStatusCode(0x08);
		I2CTransmit(slaveAddress | 1);								//Read Command
		TWCR = 1<<TWINT | 1<<TWEN;
		
		checkStatusCode(0x40);
		
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWEA;			
		checkStatusCode(0x50);
		depolymentStatus = TWDR;									//Deployment Switch State data
		
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWEA;
		checkStatusCode(0x50);
		currentSenosor_1 = TWDR;									//Current Sensor 1 Data
		
		
		TWCR = 1<<TWINT | 1<<TWEN | 0<<TWEA;
		checkStatusCode(0x58);
		currentSenosor_2 = TWDR;									//Current Sensor 2 data
		
		TWCR = 1<<TWSTO | 1<<TWINT | 1<<TWEN;						//Stop
		
		
		
																	//Displaying Received data on portB
		PORTB = depolymentStatus;
		_delay_ms(200);
		PORTB = currentSenosor_1;
		_delay_ms(200);
		PORTB = currentSenosor_2;
		_delay_ms(200);
		
    }	
	
}

