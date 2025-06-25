#include "states.h"

enum timerInterruptFlag FLAG_INT = DISABLED;
enum stateFlag FLAG = DEFAULT;

static volatile uint8_t seg_Alarm = 0;
static volatile uint8_t min_Alarm = 0; //Estas 3 son para guardar el valor de la alarma
static volatile uint8_t hora_Alarm = 0;
static volatile uint8_t alarm_Activa = 0;//Este es un flag que indica si hay una alarma activa
static volatile uint8_t alarma_contador = 0;// Aca cuento para enviar el msj ALARMA 5 veces
static volatile uint8_t alarma_sonando = 0; //Aca indico que se llego a la alarma establecida y que debe sonar las 4 próximas llamadas a ON


void task_INIT(){
	UART_INIT();
	i2c_init();
	timer1_CONFIG();
	sei();
}

void task_PRINT(){
	enviar_mensaje("Seleccione una opcion\r\n");
	enviar_mensaje("ON\r\n");
	enviar_mensaje("SET TIME\r\n");
	enviar_mensaje("SET ALARM\r\n");
	enviar_mensaje("OFF\r\n");
	
}

void task_WAIT(){
	
	int termine=0;
	
	UCSR0B|=(1<<RXCIE0);
	
	while (!termine){
		if((RX_BUFFER[rx_posicion.indice_lectura ]=='O')&&(RX_BUFFER[(rx_posicion.indice_lectura+1)%TAMANIO_RX]=='N')&&(RX_BUFFER[(rx_posicion.indice_lectura+2)%TAMANIO_RX]=='\r')){
			termine=1;
			rx_posicion.indice_lectura=(rx_posicion.indice_lectura + 3) % TAMANIO_RX;
			FLAG_INT = ENABLED;
			FLAG = ON;
		}
		else if((RX_BUFFER[rx_posicion.indice_lectura ]=='O')&&(RX_BUFFER[(rx_posicion.indice_lectura+1)%TAMANIO_RX]=='F')&&(RX_BUFFER[(rx_posicion.indice_lectura+2)%TAMANIO_RX]=='F')&&(RX_BUFFER[(rx_posicion.indice_lectura+3)%TAMANIO_RX]=='\r')){
			termine=1;
			rx_posicion.indice_lectura=(rx_posicion.indice_lectura + 4) % TAMANIO_RX;
			FLAG_INT = DISABLED;
			FLAG = OFF;
		}
		else if((RX_BUFFER[rx_posicion.indice_lectura] == 'S') &&(RX_BUFFER[(rx_posicion.indice_lectura + 1) % TAMANIO_RX] == 'E') &&(RX_BUFFER[(rx_posicion.indice_lectura + 2) % TAMANIO_RX] == 'T') &&(RX_BUFFER[(rx_posicion.indice_lectura + 3) % TAMANIO_RX] == ' ') &&(RX_BUFFER[(rx_posicion.indice_lectura + 4) % TAMANIO_RX] == 'A') && (RX_BUFFER[(rx_posicion.indice_lectura + 5) % TAMANIO_RX] == 'L') &&(RX_BUFFER[(rx_posicion.indice_lectura + 6) % TAMANIO_RX] == 'A') && (RX_BUFFER[(rx_posicion.indice_lectura + 7) % TAMANIO_RX] == 'R') && (RX_BUFFER[(rx_posicion.indice_lectura + 8) % TAMANIO_RX] == 'M') && (RX_BUFFER[(rx_posicion.indice_lectura + 9) % TAMANIO_RX] == '\r')){
			termine = 1;
			rx_posicion.indice_lectura=(rx_posicion.indice_lectura + 10) % TAMANIO_RX;
			FLAG = SET_ALARM;
		}
		else if((RX_BUFFER[rx_posicion.indice_lectura] == 'S') &&(RX_BUFFER[(rx_posicion.indice_lectura + 1) % TAMANIO_RX] == 'E') &&(RX_BUFFER[(rx_posicion.indice_lectura + 2) % TAMANIO_RX] == 'T') &&(RX_BUFFER[(rx_posicion.indice_lectura + 3) % TAMANIO_RX] == ' ') &&(RX_BUFFER[(rx_posicion.indice_lectura + 4) % TAMANIO_RX] == 'T') && (RX_BUFFER[(rx_posicion.indice_lectura + 5) % TAMANIO_RX] == 'I') &&(RX_BUFFER[(rx_posicion.indice_lectura + 6) % TAMANIO_RX] == 'M') && (RX_BUFFER[(rx_posicion.indice_lectura + 7) % TAMANIO_RX] == 'E') && (RX_BUFFER[(rx_posicion.indice_lectura + 8) % TAMANIO_RX] == '\r')){
			termine = 1;
			rx_posicion.indice_lectura=(rx_posicion.indice_lectura +9) % TAMANIO_RX;
			FLAG = SET_TIME;
		}
		else{
			rx_posicion.indice_lectura = (rx_posicion.indice_lectura + 1) % TAMANIO_RX;
		}
	}
	
	UCSR0B &= ~(1<<RXCIE0);
};

void task_ON(){
	
	char mensaje[20];
	uint8_t s,min,h,d,mes,a;
	get_time(&s,&min,&h,&d,&mes,&a);
	
	uint8_t st = bcd_to_decimal(s);
	uint8_t mt = bcd_to_decimal(min);
	uint8_t ht = bcd_to_decimal(h);
	
	if(alarm_Activa == 1 && (ht == hora_Alarm) && (mt == min_Alarm) && (st == seg_Alarm)) {
		alarma_sonando = 1;
		alarm_Activa = 0; // Desactivo la alarma para que no vuelva a activarse hasta que el usuario la setee de nuevo
		alarma_contador = 0;
	}
	if(alarma_sonando){
		if (alarma_contador < 5) {
			enviar_mensaje("ALARMA!\r\n");
			alarma_contador++;
		} else {
			alarma_sonando = 0;
			alarma_contador = 0;
		}
	}
	if(alarma_sonando==0){
		build_msg(mensaje,&s,&min,&h,&d,&mes,&a);
		enviar_mensaje(mensaje);
	}	
	
}

void task_SET_TIME(){
	
	char datos[17];
	char act;
	UCSR0B|=(1<<RXCIE0);
	enviar_mensaje("Ingrese Tiempo:\r\n");
	enviar_mensaje("HH:MM:SS DD/MM/AA\r\n");
	
	uint8_t control = 0;
	uint8_t i = 0;
	
	while(control == 0){
		
		while(rx_posicion.indice_escritura != rx_posicion.indice_lectura){
			
			act = RX_BUFFER[rx_posicion.indice_lectura];
			
			if(act == 0x0D){
				
				control = 1;
				
			}else{
				
				datos[i] = act;
				i++;
				
			}
			rx_posicion.indice_lectura = (rx_posicion.indice_lectura + 1) % TAMANIO_RX;
			
		}
		
		
	}
	
	// Armo los valores a mano
	uint8_t h   = (datos[0] - '0') * 10 + (datos[1] - '0');
	uint8_t min = (datos[3] - '0') * 10 + (datos[4] - '0');
	uint8_t s   = (datos[6] - '0') * 10 + (datos[7] - '0');
	uint8_t d   = (datos[9] - '0') * 10 + (datos[10] - '0');
	uint8_t mes = (datos[12] - '0') * 10 + (datos[13] - '0');
	uint8_t a   = (datos[15] - '0') * 10 + (datos[16] - '0');
	
	if (h > 23 || min > 59 || s > 59 || d == 0 || d > 31 || mes == 0 || mes > 12) {
		enviar_mensaje("Valores invalidos\r\n");
		return;
	}
	if (datos[2] != ':' || datos[5] != ':' || datos[8] != ' ' || datos[11] != '/' || datos[14] != '/') {
		enviar_mensaje("Error de formato\r\n");
		return;
	}

	// Convierto a BCD
	set_time(decimal_to_bcd(s), decimal_to_bcd(min), decimal_to_bcd(h),decimal_to_bcd(d), decimal_to_bcd(mes), decimal_to_bcd(a));
	enviar_mensaje("Fecha y hora seteadas!\r\n");
	
}

void task_SET_ALARM(){
	
	enviar_mensaje("Ingrese Alarma:\r\n");
	enviar_mensaje("HH:MM:SS\r\n");
	char datos[8];
	char act;
	UCSR0B|=(1<<RXCIE0);
	uint8_t control = 0;
	uint8_t i = 0;
	
	while(control == 0){
		
		while(rx_posicion.indice_escritura != rx_posicion.indice_lectura){
			
			act = RX_BUFFER[rx_posicion.indice_lectura];
			
			if(act == 0x0D){
				
				control = 1;
				
				}else{
				
				datos[i] = act;
				i++;
				
			}
			rx_posicion.indice_lectura = (rx_posicion.indice_lectura + 1) % TAMANIO_RX;
			
		}
		
		
	}
	
	hora_Alarm = (datos[0] - '0') * 10 + (datos[1] - '0');
	min_Alarm = (datos[3] - '0') * 10 + (datos[4] - '0');
	seg_Alarm = (datos[6] - '0') * 10 + (datos[7] - '0');
	alarm_Activa = 1; // Activo la alarma
	alarma_sonando = 0;
	alarma_contador = 0;
	enviar_mensaje("Alarma activada!\r\n");
	
}

void timer1_CONFIG(){
	
	TCCR1B |= (1 << WGM12);					//Modo CTC (Clear Timer on Compare Match)
	TCCR1B |= (1 << CS12) | (1 << CS10);	//Prescaler 1024
	OCR1A = 15624;							//Valor de comparación para 1 segundo
	TCNT1 = 0;								// Inicializar contador
	
}

void timer1_ENABLE() {
	
	if(FLAG_INT == ENABLED){
		TIMSK1 |= (1 << OCIE1A);			//Habilitar interrupción por comparación A
	}
	if(FLAG_INT == DISABLED){
		TIMSK1 &= ~(1 << OCIE1A);			//Deshabilito la interrupcion por comparacion
	}
	
}