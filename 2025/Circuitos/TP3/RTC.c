#include "RTC.h"

void rtc_write_byte(uint8_t direccion, uint8_t dato){
    i2c_start();
    i2c_write(0xD0); // Dirección del RTC en modo escritura
    i2c_write(direccion);
    i2c_write(dato);
    i2c_stop();
}

uint8_t rtc_read_byte(uint8_t direccion) {
    uint8_t dato;
    i2c_start();
    i2c_write(0xD0); // Dirección del RTC en modo escritura
    i2c_write(direccion);
    i2c_stop();
    i2c_start();
    i2c_write(0xD1); // Dirección del RTC en modo lectura
    dato = i2c_read(1); // Leer byte y enviar NACK
    i2c_stop();
    return dato;
}
void rtc_write_cadena(uint8_t direccion, uint8_t *dato, uint8_t tamano ){
    i2c_start();
    i2c_write(0xD0); // Dirección del RTC en modo escritura
    i2c_write(direccion);
    for (uint8_t i = 0; i < tamano; i++)
    {
        i2c_write(dato[i]);
    }
    i2c_stop();
}