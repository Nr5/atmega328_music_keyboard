#define F_CPU 15000000
#define __AVR_ATmega328__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define BUTTON_PORT PORTC
uint8_t volume=0;
volatile uint8_t button_state[4];
volatile uint8_t number_of_notes;
ISR(TIMER0_OVF_vect){
	for (uint8_t keyboard_col=0;keyboard_col<4;keyboard_col++){
		 PORTC= ~(1<<keyboard_col);
		 button_state[keyboard_col]= ~PIND;
	}
}



int main(void) {
	TCCR0A= 0;
	TCCR0B= (1<<CS12);
	TIMSK0= (1<<TOIE0);
	TCNT0 = 0;

	DDRC=0xff;
	DDRB=0xff;
	DDRD=0;
	PORTD=0xff;
	sei();
	uint8_t freq[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	uint16_t clock =0;
	const uint16_t wavelen[12]={256,271,287,304,322,341,362,383,406,430,456,483};


		while (1){
		if (button_state[1] || button_state[2]){ 
			for (uint8_t i =0; i< 8; i++){ 
				if (button_state[1] & (0x01 << i) ){
					if (! (clock % (wavelen[i+1])*4))	{
						freq[i+1] = !freq[i+1]; 
					} 
				} 
			}
			if (button_state[2] & (0x01 << 0) && ! (clock % wavelen[0])){
				freq[0] = !freq[0];
			}
			if (button_state[2] & (0x01 << 1) && ! (clock % wavelen[9])){
				freq[9] = !freq[9];
			}
			if (button_state[2] & (0x01 << 2) && ! (clock % wavelen[10])){
				freq[10] = !freq[10];
			}
			if (button_state[2] & (0x01 << 3) && ! (clock % wavelen[11])){
				freq[11] = !freq[11];
			}
			//volume= ~volume;
//			number_of_notes = ((button_state[1] >> 0) & 1) + ((button_state[1] >> 1) & 1) + ((button_state[1] >> 2) & 1) + ((button_state[1] >> 3) & 1) +((button_state[1] >> 4) & 1) + ((button_state[1] >> 5) & 1) + ((button_state[1] >> 6) & 1) + ((button_state[1] >> 7) & 1) +((button_state[2] >> 0) & 1) + ((button_state[2] >> 1) & 1) + ((button_state[2] >> 2) & 1) + ((button_state[2] >> 3) & 1);
//			uint16_t volume = (freq[0]+freq[1]+freq[2]+freq[3]+freq[4]+freq[5]+freq[6]+freq[7]+freq[8]+freq[9]+freq[10]+freq[11]) * 255 / number_of_notes;
			PORTB=(freq[0]+freq[1]+freq[2]+freq[3]+freq[4]+freq[5]+freq[6]+freq[7]+freq[8]+freq[9]+freq[10]+freq[11]) * 21;
			//PORTB=volume;
		}
		clock ++;
	}

	return 0;
}
