/*
 *
 * TP3_ENTREGABLE
 * Reloj Calendario con Alarma
 * Arduino UNO, RTC DS3231 
 * Circuitos Digitales y Microcontroladores
 * Arpone, David. Iloro, Gonzalo. Lacourrage, Santiago.
 * 24/06/2025
 *
 */ 

#include "main.h"

enum interruptFlagTimer1 FLAG_TIMER = TIMER_OFF;
enum bussyFlag FLAG_BUSY = IDLE;

int main(){
	
	rx_posicion.indice_escritura = 0;
	rx_posicion.indice_lectura = 0;
	tx_posicion.indice_lectura = 0;
	tx_posicion.indice_escritura = 0;
	
	task_INIT();
	task_PRINT();
	
	while (1) {
		
		
		UCSR0B|=(1<<RXCIE0); //Revisar, no tendria que estar aca pero el off a veces se rompe y produce deathlock
		
		if(FLAG == WAIT){
			
			task_WAIT();
			
		}
		if(FLAG_TIMER == TIMER_ON && FLAG_BUSY == IDLE){
			
			task_ON();
			FLAG_TIMER = TIMER_OFF;	//Limpia FLAG de temporizador
			
		}
		if(FLAG == ON || FLAG == OFF){
			
			timer1_ENABLE();
			FLAG = DEFAULT;
			
		}
		if(FLAG == SET_ALARM && FLAG_BUSY == IDLE){
			
			task_SET_ALARM();
			FLAG = DEFAULT;
			
		}
		if(FLAG == SET_TIME && FLAG_BUSY == IDLE){
			
			task_SET_TIME();
			FLAG = DEFAULT;
			
		}
		
	}
}

ISR(USART_RX_vect){
	
	FLAG_BUSY = BUSY;
	
	uint8_t dato_recibido = UDR0; //Limpiamos registro para nueva interrupcion
		
	if(dato_recibido == 0x08){
		rx_posicion.indice_escritura = ((rx_posicion.indice_escritura - 1) % TAMANIO_RX);
	}else{
	
	uint8_t siguiente = (rx_posicion.indice_escritura + 1) % TAMANIO_RX;
	if(!(siguiente == rx_posicion.indice_lectura)){
		RX_BUFFER[rx_posicion.indice_escritura] = dato_recibido; //Descarta el dato si el buffer esta lleno
		rx_posicion.indice_escritura = siguiente;
	}
	if(dato_recibido == 0x0D){
		FLAG = WAIT;
		FLAG_BUSY = IDLE;
		FLAG_TIMER = TIMER_OFF;
	}
	}
	UDR0 = dato_recibido;
	UCSR0B |= (1<<UDRIE0);
	
}


ISR(USART_UDRE_vect){
		
	if(tx_posicion.indice_lectura != tx_posicion.indice_escritura){
		
		UDR0 = TX_BUFFER[tx_posicion.indice_lectura];
		tx_posicion.indice_lectura = (tx_posicion.indice_lectura + 1) % TAMANIO_TX;
		
	}else{
		
		UCSR0B &= ~(1<<UDRIE0); // cuando mandas el ultimo byte tenes que deshabilitar la interrupcion de buffer vacio
		//UCSR0B|=(1<<RXCIE0);
		
	}

}
	
ISR(TIMER1_COMPA_vect){
	
	if(FLAG_TIMER == TIMER_OFF){
		FLAG_TIMER = TIMER_ON;
	}
	
}

