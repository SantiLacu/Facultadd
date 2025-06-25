#ifndef UART_H
#define UART_H

#include "main.h"

struct Buffer {
	uint8_t indice_escritura;
	uint8_t indice_lectura;
};

extern volatile uint8_t RX_BUFFER[TAMANIO_RX]; //Data bits + control bit
extern volatile uint8_t TX_BUFFER[TAMANIO_TX]; //N-1 bits data
extern volatile struct Buffer rx_posicion;
extern volatile struct Buffer tx_posicion;

void UART_INIT();
void enviar_mensaje(uint8_t *msg);
void parsing_bcd_to_decimal(uint8_t *s, uint8_t *min, uint8_t *h, uint8_t *d, uint8_t *mes, uint8_t *a);
uint8_t decimal_to_bcd(uint8_t decimal);
void parsing_decimal_to_bcd(uint8_t *s, uint8_t *min, uint8_t *h, uint8_t *d, uint8_t *mes, uint8_t *a);
uint8_t bcd_to_decimal(uint8_t bcd);
void build_msg(char* buffer_mensaje,uint8_t *s, uint8_t *min, uint8_t *h, uint8_t *d, uint8_t *mes, uint8_t *a);

#endif