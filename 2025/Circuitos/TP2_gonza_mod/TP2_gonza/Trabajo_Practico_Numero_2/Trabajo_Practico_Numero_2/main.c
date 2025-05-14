/*
 * Trabajo_Practico_Numero_2.c
 *
 * Created: 28/4/2025 12:55:23
 * Author : gonzalo iloro
 */ 


#include "main.h"
#include "keypad.h"
#define LCD_4bit

int main(void)
{
	const char keymap[16] = {
		'1','4','7','0',
		'2','5','8','*',
		'3','6','9','#',
		'A','B','C','D'
	};
	TCCR0A=0x02;
	TCCR0B=0x02;
	OCR0A=
	
	
	uint8_t lectura = 0xFF;

	LCDinit();          // Inicializá el LCD una sola vez fuera del loop
	_delay_ms(100);
	LCDGotoXY(0,0);
	while (1)
	{		
		if (KEYPAD_Scan(&lectura))  // Devuelve 1 si se detectó una tecla nueva
		{
			if (lectura < 16)       // Asegura que el índice esté dentro del keymap
			{
				LCDsendChar(keymap[lectura]);       // Muestra la tecla presionada
				LCDGotoXY(0,0);
			}
		}
	}
}
