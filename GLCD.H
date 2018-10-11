#define F_CPU 1000000UL
#include<avr/io.h>
#include<util/delay.h>


#define lcd PORTC
#define rs  0
#define rw  1
#define en  2

#define YES 8
#define NO	9

void lcd_init();
void lcd_command(unsigned char);
void lcd_data(unsigned char);
void lcd_string(unsigned char *str);
void lcd_number(unsigned int);
void lcd_clear(unsigned char pos);
void adc_init();
unsigned int adc_read(unsigned char channel);

int main()
{
	unsigned int sensor;
	DDRA = 0x00;
	DDRC = 0xFF;

	lcd_init();
	adc_init();
	
	while(1)
	{

	sensor=adc_read(0);
	
	lcd_command(0x80);
	lcd_string("SENSOR=   ");
	lcd_command(0x85);
	lcd_number(sensor);

	_delay_ms(10);
 	}
	return 0;
}

void lcd_init()
{
	_delay_ms(5);
	lcd_command(0x02);
 	lcd_command(0x28);
	lcd_command(0x06);
	lcd_command(0x0c);
}
void lcd_command(unsigned char com)
	{
	lcd = com & 0xF0;		//send higher bit

	lcd &= ~(1<<rs); 		//rs =0
	lcd &= ~(1<<rw);		//rw =0
	lcd |=(1<<en);			//en =1
	_delay_ms(1);
	lcd &= ~(1<<en);		//en =0

	lcd = (com<<4) & 0xF0;	//send lower bit

	lcd &= ~(1<<rs); 		//rs =0
	lcd &= ~(1<<rw);		//rw =0
	lcd |=(1<<en);			//en =1
	_delay_ms(1);
	lcd &= ~(1<<en);		//en =0
	}
void lcd_data(unsigned char value)
	{

	lcd =value & 0xF0;		//send higher bit

	lcd |= (1<<rs); 		//rs =1
	lcd &= ~(1<<rw);		//rw =0
	lcd |=(1<<en);			//en =1
	_delay_ms(1);
	lcd &= ~(1<<en);		//en =0


	lcd =(value<<4) & 0xF0;	//send lower bit

	lcd |= (1<<rs); 		//rs =1
	lcd &= ~(1<<rw);		//rw =0
	lcd |=(1<<en);			//en =1
	_delay_ms(1);
	lcd &= ~(1<<en);		//en =0
	}

void lcd_string(unsigned char *str)
{
	char i=0;
	while(str[i]!='\0')
	{
	lcd_data(str[i]);
	i++;
	}
}
void lcd_number(unsigned int value)
{
	unsigned int d=0;
	lcd_command(0x04);	//auto decrement mode

	if(value==0)
	lcd_data(value+48);

	while(value!=0)
	{
	d=value%10;
	lcd_data(d+48);
	value=value/10;
	}
	lcd_command(0x06);	//auto increment mode
}
void lcd_clear(unsigned char pos)
{
	if(pos==1)
	{
	lcd_command(0x80);
	lcd_string("                ");
	}
	else if(pos==2)
	{
	lcd_command(0xc0);
	lcd_string("                ");
	}
	else if(pos==3)
	{
	lcd_command(0x80);
	lcd_string("                ");
	lcd_command(0xc0);
	lcd_string("                ");
	}

}
void adc_init()
{
	ADMUX = 0B01000000;
	ADCSRA = ((1<<ADEN) | (1<<ADPS2)| (1<<ADPS1)| (1<<ADPS0));
	}
unsigned int adc_read(unsigned char channel)
{	
	
	ADMUX = 0x40|channel; 
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));

	ADCSRA |= (1<<ADIF);
	return ADC;
}
