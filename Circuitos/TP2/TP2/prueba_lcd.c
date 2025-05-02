#include <avr/io.h>
#include "lcd.h"

int main(void) {
	LCDinit();                      // Inicializa LCD
	LCDstring((uint8_t*)"RV1: 73%", 8);  // Muestra el valor del potenciómetro
	while (1) {
		// El potenciómetro se ajusta manualmente en Proteus
	}
}