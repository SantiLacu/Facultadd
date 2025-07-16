/*
Utilizando el modelo de Proteus MOTOR-PWMSERVO, realizar un programa que permita controlar por medio
de la técnica PWM el ángulo de giro de un eje entre +90° y -90°. Analizar las propiedades del modelo estándar y
determinar la resolución en el control del ángulo que se puede obtener utilizando fast PWM de 16 bits.

*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#define  F_CPU 16000000 UL
int main (void){
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM12) | (1<<WGM13);  //Seteo el Modo PWM 14
    TCCR1B |= (1<<CS11); // Prescaler 8
	/*
	Un ancho de pulso típico de 1.5 ms posiciona el servomotor en su posición central (0°). 
	Para los extremos, un pulso de aproximadamente 0.5 ms lo llevaría a un extremo (-90°), y un pulso de 2.5 ms lo llevaría al otro extremo (+90°). 
	Los servomotores estándar esperan una señal PWM con un período de aproximadamente 20 ms= 50Hz
	TOP = (16,000,000 Hz / (8 * 50 Hz))-1 = (16,000,000 / 400)-1 = 39.999 
	PW = (OCR1x + 1) * T_period / (TOP + 1)--> OCR1A = ((PW * (TOP+1))/T_period)-1 --> OCR1A_min = ((0.5ms * (40.000))/20ms)-1 = 999
	OCR1A_max= ((2.5ms * (40.000))/20ms)-1 = 4999
	*/
	uint16_t p=999;
	ICR1=39999;
	OCR1A=p;
	DDRB |= (1<<PINB1);
	while (1){
		_delay_ms(100);
		if(p++>=4999){
			p=999;
		}
		OCR1A=p;
	}
}