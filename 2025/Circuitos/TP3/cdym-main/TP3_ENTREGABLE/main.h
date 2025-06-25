
#ifndef main_H
#define main_H

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#define TAMANIO_RX 18
#define TAMANIO_TX 32

#include "I2C.h"
#include "UART.h"
#include "states.h"
#include "RTC.h"

enum interruptFlagTimer1{
	TIMER_ON, TIMER_OFF
} FLAG_TIMER;

enum bussyFlag{
	BUSY, IDLE
}FLAG_BUSY;

#endif