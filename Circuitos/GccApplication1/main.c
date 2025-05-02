/*
 * Trabajo_Practico_Numero_2.c
 *
 * Created: 28/4/2025 12:55:23
 * Author : gonzalo iloro
 */ 
#include "main.h"
#include "lcd.h"


#define KP_PORT(Data) { PORTB = ((Data & (1<<7)) >> 3) | ((Data & (1<<6)) >> 3)| ((Data & (1<<5)) >> 5); PORTD |= ((Data & (1<<4)) << 3) | ((Data & (1<<3)) >> 0)| ((Data & (1<<2)) << 3)| ((Data & (1<<1)) << 3)| ((Data & (1<<0)) << 2);}
//#define KP_PIN(Data) { PINB |= ((Data & (1<<7)) >> 3) | ((Data & (1<<6)) >> 3)| ((Data & (1<<5)) >> 5);PIND = (PIND & 0xF9) | ((Data & (1<<4)) << 3) | ((Data & (1<<3)) >> 0)| ((Data & (1<<2)) << 3)| ((Data & (1<<1)) << 3)| ((Data & (1<<0)) << 2);}
#define KP_pin ((PINB4<<3) | (PINB3<<3) | (PINB0 << 5) | (PIND7 >> 3) | (PIND3 << 0) | (PIND5 >> 2) | (PIND4 >> 2) | (PIND2 >> 2))
#define KP_DDR(Data) { DDRB |= ((Data & (1<<7)) >> 3) | ((Data & (1<<6)) >> 3)| ((Data & (1<<5)) >> 5);DDRD = (PIND & 0xF9) | ((Data & (1<<4)) << 3) | ((Data & (1<<3)) >> 0)| ((Data & (1<<2)) << 3)| ((Data & (1<<1)) << 3)| ((Data & (1<<0)) << 2);}
//Hace falta inicializar?
#define  LCD_4bit


uint8_t KeypadUpdate(void);
uint8_t KEYPAD_Scan (uint8_t *pkey);
int main(void)
{	uint8_t lectura=0xFF;
    LCDinit();

	while (1) 
    {
		
		while (lectura==0xFF){
			KEYPAD_Scan(&lectura);
		}
			
    }
}





uint8_t KeypadUpdate(void){
	
	uint8_t r, c;
	
	char pepe[4][4] = {{'1', '2', '3', 'A'},{'4', '5', '6', 'B'},{'7', '8', '9', 'C'},{'*', '0', '#', 'D'}};

	KP_PORT(0x0F);  // Habilita las resistencias de pull-up internas en los 4 pines bajos de PORTB (colocándolos en alto).

	for(c = 0; c < 4; c++)
	{
		KP_DDR(0);        // Pone todos los pines de PORTB como entradas (por defecto).

		KP_DDR(0x80 >> c);    // Pone un solo pin como salida (una columna activa a la vez).

		for(r = 0; r < 4; r++)
		{	
			if(!(KP_pin & (0x08 >> r)))  // Lee los 4 pines de las filas.
			{
				//return (r * 4 + c);
				return pepe[r*4][c];    // Retorna un número que identifica qué tecla se presionó.
			}
		}
	}
	return 0xFF;  // Si ninguna tecla fue presionada, retorna 0xFF.
}
uint8_t KEYPAD_Scan (uint8_t *pkey)
{
	static uint8_t Old_key, Last_valid_key=0xFF; // no hay tecla presionada;
	uint8_t Key;
	
	Key= KeypadUpdate();
	if(Key==0xFF){
		Old_key=0xFF;	// no hay tecla presionada
		Last_valid_key=0xFF;
		return 0;
	}
	if(Key==Old_key) { //2da verificación
		if(Key!=Last_valid_key){ //evita múltiple detección
			*pkey=Key;
			Last_valid_key = Key;
			return 1;
		}
	}
	Old_key=Key; //1era verificación
	return 0;
}