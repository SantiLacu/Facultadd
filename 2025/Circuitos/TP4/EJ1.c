/*Realice un programa que permita generar una señal PWM con el TIMER 0 del MCU. El valor del ciclo de
trabajo (entre 0 y 99%) deberá enviarse al MCU desde la terminal serie. Utilice el modo 7 fast PWM para generar
los 100 ciclos de trabajo. Muestre cuales son las posibles frecuencias de PWM en base a la configuración del
prescalador, elija uno para su implementación. Simule en Proteus y verifique la señal mediante el osciloscopio.*/

#include <avr/io.h>
#include "UART.h"
#include <stdlib.h>
#define F_CPU 16000000UL // Definir la frecuencia del CPU (ej. 16 MHz)

// Función para leer un número entre 0 y 99 desde la UART

uint8_t leer_numero_uart() {
	char buffer[3] = {0};
	uint8_t i = 0;
	char c;
	while (i < 2) { // Máximo 2 dígitos
		c = receive_char();
		if (c == '\r' || c == '\n') break; // Enter
		if (c >= '0' && c <= '9') {
			buffer[i++] = c;
		}
	}
	buffer[i] = '\0';
	return (uint8_t)atoi(buffer);
}

int main(void) {
	UART_INIT();
	// Configurar los pines PD6 (OC0A) como salidas
	// ya que OC0A está asociado con PD6 en el ATmega328P [3]
	DDRD |=(1 << PIND6);
	// Configuración para OC0A (Polaridad Normal):
	// COM0A1:0 = 10 (Clear OC0A en Compare Match, Set OC0A en BOTTOM).
	// Esto se logra activando el bit COM0A1 y desactivando COM0A0. [1]
	TCCR0A |= (1 << COM0A1);
	TCCR0A &= ~(1 << COM0A0);

	// Adicionalmente, para que estas configuraciones de polaridad funcionen
	// correctamente en un contexto de PWM, se debe seleccionar un modo de operación PWM.
	// Por ejemplo, para Fast PWM (TOP=0xFF), se configurarían los bits WGM01:00 = 11 en TCCR0A. [4]
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	// Configurar el prescalador para iniciar el Timer0 (por ejemplo, sin prescalador) [3]
	// Esto se hace en el registro TCCR0B.
	TCCR0B |= (1 << CS00); // Sin prescalador (f_clk/1)
	//Pido al usuario que ingrese el valor del ciclo de trabajo entre 0 y 99%
	enviar_mensaje("Ingrese el ciclo de trabajo de 0 a 99 \r\n");
	uint8_t rsp = leer_numero_uart();
	OCR0A = (rsp*255)/100;
	while (1) {

	}

	return 0;
}