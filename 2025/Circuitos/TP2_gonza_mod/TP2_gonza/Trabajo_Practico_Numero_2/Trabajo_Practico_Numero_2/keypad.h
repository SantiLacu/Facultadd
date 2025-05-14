#ifndef KEYPAD_H
#define KEYPAD_H
#ifndef MAIN_H
#include "main.h"
#endif
//Defino la macro para los puertos virtuales del KeyPad
#define DDRv(DATA) DDRB = (DDRB & ~0x19) | ( ((DATA & (1<<7)) >> 3) | ((DATA & (1<<6)) >> 3) | ((DATA & (1<<5)) >> 5) ); DDRD = (DDRD & ~0xBC) | ( ((DATA & (1<<4)) << 3) ) | ((DATA & (1<< 3)) << 0) | ((DATA & (1<<2)) << 3) | ((DATA & (1<<1)) << 3) | ((DATA & (1<<0)) << 2);
#define PORTv(DATA) PORTB =  (PORTB & ~0x19) | ( ((DATA & (1<< 7)) >> 3) | ((DATA & (1<<6)) >> 3) | ((DATA & (1<<5)) >> 5)); PORTD = (PORTB & ~0x19) | ((DATA & (1<<4))<< 3)| ( ((DATA & (1<< 3)) << 0) | ((DATA & (1<<2)) << 3) | ((DATA & (1<<1)) << 3) | ((DATA & (1<<0))<< 2) );
#define PINv() (((PIND & (1<< 3)) >>0) | ((PIND & (1<<5)) >>3) | ((PIND & (1<<4)) >> 3) | ((PIND &(1<<2)) >> 2))
uint8_t KeypadUpdate(void);
uint8_t KEYPAD_Scan (uint8_t *pkey);
#endif