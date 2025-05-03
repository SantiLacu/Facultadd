#ifndef KEYPAD_H
#define KEYPAD_H
#ifndef VIRTUAL_PORT_H
#define VIRTUAL_PORT_H
#ifndef MAIN_H
	#include "main.h"
#endif

	// Escritura en puerto virtual
	#define KP_PORT(Data) {PORTB = (PORTB & 0xC6) | (((Data) & (1<<7)) >> 3) | (((Data) & (1<<6)) >> 3) | (((Data) & (1<<5)) >> 5); PORTD = (PORTD & 0x06) | (((Data) & (1<<4)) << 3) | (((Data) & (1<<3)) << 0) | (((Data) & (1<<2)) << 3) | (((Data) & (1<<1)) << 3) | (((Data) & (1<<0)) << 2);}

	// Lectura de puerto virtual
	#define KP_PIN() ( (((PINB & (1<<4)) << 3) | ((PINB & (1<<3)) << 3) | ((PINB & (1<<0)) << 5) | ((PIND & (1<<7)) >> 3) | ((PIND & (1<<3)) >> 0) | ((PIND & (1<<5)) >> 2) | ((PIND & (1<<4)) >> 2) | ((PIND & (1<<2)) >> 2)) )

	// Configuración de dirección
	#define KP_DDR(Data){ DDRB = (DDRB & 0xC6) | (((Data) & (1<<7)) >> 3) | (((Data) & (1<<6)) >> 3) | (((Data) & (1<<5)) >> 5); DDRD = (DDRD & 0x06) | (((Data) & (1<<4)) << 3) | (((Data) & (1<<3)) << 0) | (((Data) & (1<<2)) << 3) | (((Data) & (1<<1)) << 3) | (((Data) & (1<<0)) << 2);}

#endif // VIRTUAL_PORT_H
uint8_t KEYPAD_Scan(uint8_t *pkey);
/************************************************************************/
/* Identifica la celda del panel seleccionada y devuelve su posicion    */
/************************************************************************/
uint8_t KEYPAD_Update(void);
/************************************************************************/
/* Evita la lectura multible y el rebote                                */
/************************************************************************/
#endif