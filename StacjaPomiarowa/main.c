#define F_CPU 16000000

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

void ADC_Init(void)
{
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

long int getADC(char channel)
{
	long int W = 0;
	ADMUX |= channel;
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADIF));
	ADCSRA |= (1 << ADIF);
	W = ADCL;
	W |= (ADCH << 8);
	return W;
}
void SendChar(unsigned char dana)
{
	DDRD = 0b00111111;
	PORTD |= 0b00110000; //RS = 1 i E = 1
	PORTD = ((PORTD & 0b11110000) | ((dana>>4) & 0b00001111));
	PORTD &= 0b11101111; //E = 0
	_delay_loop_2(5000);
	PORTD |= 0b00010000; //E = 1
	PORTD = ((PORTD & 0b11110000) | (dana & 0b00001111));
	PORTD &= 0b11101111; //E = 0
	_delay_loop_2(5000);
	PORTD |= 0b00010000; //E = 1
}
void SendCommand(unsigned char dana)
{
	DDRD = 0b00111111;
	PORTD &= 0b11011111; //RS = 0
	PORTD |= 0b00010000; //E = 1
	PORTD = ((PORTD & 0b11110000) | ((dana>>4) & 0b00001111));
	PORTD &= 0b11101111; //E = 0
	_delay_loop_2(25000);
	PORTD |= 0b00010000; //E = 1
	PORTD = ((PORTD & 0b11110000) | (dana & 0b00001111));
	PORTD &= 0b11101111; //E = 0
	_delay_loop_2(25000);
	PORTD |= 0b00010000; //E = 1
}

void LCD_Init(void)
{
	SendCommand(0x33);
	SendCommand(0x32);
	SendCommand(0x28);
	SendCommand(0x08);
	SendCommand(0x01);
	SendCommand(0x06);
	SendCommand(0x0F);
}

int main(void)
{
	LCD_Init();
	ADC_Init();
	char temperature[20];
	char temperature2[20];	
    while (1) {	
		SendCommand(0x01);
		long int result = getADC(0);
		result = (result * 50000)/1024;
		ltoa(result, temperature, 10);
		SendChar(temperature[0]);
		SendChar(temperature[1]);
		SendChar(',');
		SendChar(temperature[2]);
		
		_delay_ms(100);		
		
    }
}
