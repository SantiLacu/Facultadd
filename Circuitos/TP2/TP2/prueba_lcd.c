/*
 * TP2.c
 *
 * Created: 28/4/2025 20:44:35
 * Author : Usuario
 */ 

#include "main.h"
#include "kp.h"
#include "lcd.h"
#include <string.h>
#define LCD_4bit

int main(void)
{	uint8_t lectura;
	char keypad[16]={'1','2','3','A','4','5','6','B','7','8','9','C','0','*','#','D'};
	LCDinit();
	LCDGotoXY(0,2);
	LCDcursorOn();
	while (1)
	{
		while (!(KEYPAD_Scan(&lectura))){
			LCDsendChar('1');
		}
		LCDclr();
		LCDsendChar(keypad[lectura]);
		_delay_ms(300);
		LCDclr();
	}
   
}

uint8_t KeypadUpdate()
{
	uint8_t r, c;  // Variables para filas (rows) y columnas (columns)

	// Configura los pines PB0-PB3 como entrada con pull-up (filas)
	// y PB4-PB7 como salidas inicialmente en alto (columnas)
	KP_PORT(0x0F);  // Activa pull-ups en PB0-PB3

	for(c = 0; c < 4; c++)
	{
		// Configura todas las columnas como entradas (alta impedancia)
		KP_DDR(0x0F);
		
		// Configura la columna(KP0, KP1, KP2, KP3) actual como salida en bajo (0)
		KP_DDR(0x80 >> c);  // Secuencia: 0b00001000, 0b00000100, etc.
		
		// Escanea cada fila
		for(r = 0; r < 4; r++)
		{
			// Verifica si la fila(KP4, KP5, KP6, KP7) actual está en bajo (tecla presionada)
			if(!(KP_PIN()  & (0x08 >> r)))
			{
				// Retorna código de tecla: (fila*4 + columna)
				return (r * 4 + c);
			}
		}
	}
	
	return 0xFF;  // Retorna 0xFF si no hay tecla presionada
}


uint8_t KEYPAD_Scan(uint8_t *pkey){
		static uint8_t Old_key, Last_valid_key = 0xFF; // No hay tecla presionada
		uint8_t Key=KeypadUpdate();
		
		if(Key == 0xFF) {
			Old_key = 0xFF;    // No hay tecla presionada
			Last_valid_key = 0xFF;
			return 0;
		}
		
		if(Key == Old_key) {   // Segunda verificación
			if(Key != Last_valid_key) { // Evita múltiples detecciones
				*pkey = Key;
				Last_valid_key = Key;
				return 1;
			}
		Old_key = Key;         // Primera verificación
		return 0;
	}
}