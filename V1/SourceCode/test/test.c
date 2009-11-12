
#include <util/delay.h>
#include <avr/io.h>


int main()
{
	DDRB |= 1;
	//PORTB |= 1;
	
	while(1)
	{
		_delay_ms(1000);
		PORTB &= (~1);
		_delay_ms(1000);
		PORTB |= 1;
	}	


}
