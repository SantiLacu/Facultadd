#ifndef RTC_H
#define RTC_H

#include "main.h"
#include "I2C.h"

void rtc_write_byte(uint8_t direccion, uint8_t dato);
uint8_t rtc_read_byte(uint8_t direccion);
void rtc_write_cadena(uint8_t direccion, uint8_t *dato, uint8_t tamano );
#endif
