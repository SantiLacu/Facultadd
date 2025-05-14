
#include "keypad.h"
uint8_t KeypadUpdate(void){
	
	uint8_t r, c;

	PORTv(0x0F);  // Habilita las resistencias de pull-up internas en los 4 pines bajos de PORTB (colocándolos en alto).

	for(c = 0; c < 4; c++)
	{
		DDRv(0x00);        // Pone todos los pines de PORTB como entradas (por defecto).

		DDRv(0x80 >> c);    // Pone un solo pin como salida (una columna activa a la vez).
		for(r = 0; r < 4; r++)
		{
			if(!(PINv() & (0x08 >> r)))  // Lee los 4 pines de las filas.
			{
				return (r * 4 + c);    // Retorna un número que identifica qué tecla se presionó.
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