#include "UART.h"

volatile uint8_t RX_BUFFER[TAMANIO_RX]; //Data bits + control bit
volatile uint8_t TX_BUFFER[TAMANIO_TX]; //N-1 bits data
volatile struct Buffer rx_posicion;
volatile struct Buffer tx_posicion;

void UART_INIT(){
    DDRD |= (1 << PIND1);				//PIND1 OUTPUT (TX)
	DDRD &= ~(1 << PIND0);				//PIND0 INPUT(RX)
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);//8 bits Transmission
	UBRR0L = 103;						//9600Hz
	UCSR0B |= (1<<TXEN0)|(1<<RXEN0);	//Enabled Transmission and Reception

}
void parsing_bcd_to_decimal(uint8_t *s, uint8_t *min, uint8_t *h, uint8_t *d, uint8_t *mes, uint8_t *a){
	 *s=bcd_to_decimal(*s);
	 *min=bcd_to_decimal(*min);
	 *h=bcd_to_decimal(*h);
	 *d=bcd_to_decimal(*d);
	 *mes=bcd_to_decimal(*mes);
	 *a=bcd_to_decimal(*a);
 }
 void parsing_decimal_to_bcd(uint8_t *s, uint8_t *min, uint8_t *h, uint8_t *d, uint8_t *mes, uint8_t *a){
	 *s= decimal_to_bcd(*s);
	 *min= decimal_to_bcd(*min);
	 *h= decimal_to_bcd(*h);
	 *d= decimal_to_bcd(*d);
	 *mes= decimal_to_bcd(*mes);
	 *a= decimal_to_bcd(*a);
}
 uint8_t decimal_to_bcd(uint8_t decimal) {
	 return ((decimal / 10) << 4) | (decimal % 10);
	  }

uint8_t bcd_to_decimal(uint8_t bcd) {
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}
void enviar_mensaje(uint8_t *msg) {
	uint8_t i = 0;

	while (msg[i] != '\0') {

		//Wait space in buffer
		while((tx_posicion.indice_escritura + 1) % TAMANIO_TX == tx_posicion.indice_lectura) {
			//Sleep micro
		}

		TX_BUFFER[tx_posicion.indice_escritura] = msg[i];
		tx_posicion.indice_escritura = (tx_posicion.indice_escritura + 1) % TAMANIO_TX;

		i++;
	}

	UCSR0B |= (1 << UDRIE0);			//Enable Empty Buffer
}


void build_msg(char* buffer_mensaje,uint8_t *s, uint8_t *min, uint8_t *h, uint8_t *d, uint8_t *mes, uint8_t *a){
		parsing_bcd_to_decimal(s,min,h,d,mes,a);
		sprintf(buffer_mensaje, "FECHA: %02u/%02u/%02u %02u:%02u:%02u\r\n", *d, *mes, *a, *h, *min, *s);
}