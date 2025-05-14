#ifndef KEYPAD_H
	#define KEYPAD_H
	#ifndef MAIN_H
		#include "main.h"	
	#endif
	//Defino la macro para los puertos virtuales del KeyPad
	#define DDRv(DATA) DDRB = (DDRB & 0xE5) | ( ((DATA & (1<<7)) >> 3) | ((DATA & (1<<6)) >> 3) | ((DATA & (1<<5)) >> 5) ); DDRD = (DDRD & ~0x88) | ( ((DATA & (1<<4)) << 3) ); DDRD &= ~( ((DATA & (1<< 3)) << 0) | ((DATA & (1<<2)) << 3) | ((DATA & (1<<1)) << 3) | ((DATA & (1<<0)) << 2) );
	#define PORTv(DATA) PORTB = (PORTB & 0x43) | ( ((DATA & (1<< 7)) >> 3) | ((DATA & (1<<6)) >> 3) | ((DATA & (1<<5)) >> 5)); PORTD |=((DATA & (1<<4))<< 3)| ( ((DATA & (1<< 3)) << 0) | ((DATA & (1<<2)) << 3) | ((DATA & (1<<1)) << 3) | ((DATA & (1<<0))<< 2) );
	#define PINv() (((PIND & (1<< 3)) >>0) | ((PIND & (1<<5)) >>3) | ((PIND & (1<<4)) >> 3) | ((PIND &(1<<2)) >> 2))
	//PuertoVirtua(PB4, PB3, PB0, PD7, PD5, PD4, PD3, PD2) 0xB0=10110000 --> ~0xB0=01001111, para mi sería 11100110 o 0xE6 para B y para D 01000011
	//PuertoVirtual ( o, o, o, o, i, i, i, i) (Input/Output)
	//PuertoVirtual ( x, x, x, x, r, r, r, r) (read)
	uint8_t KeypadUpdate(void);
	uint8_t KEYPAD_Scan (uint8_t *pkey);
#endif