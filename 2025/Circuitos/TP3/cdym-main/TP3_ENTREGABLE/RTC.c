#include "RTC.h"

void set_time(uint8_t s, uint8_t min, uint8_t h, uint8_t d, uint8_t mes, uint8_t a){
    i2c_start();
	i2c_write(0b11010000);	//Write Device Address
	i2c_write(0b00000000);
	i2c_write(s);			//sec
	i2c_write(min);			//min
	h = h & 0b00111111;		//Set hs in 24
	i2c_write(h);			//hs
	i2c_write(0x01);		//Write DATE
	i2c_write(d);			// day
	i2c_write(mes);			// month
	i2c_write(a);			// year
	i2c_stop();
}

void get_time(uint8_t *s, uint8_t *min, uint8_t *h, uint8_t *d, uint8_t *mes, uint8_t *a){
	i2c_start();
	i2c_write(0b11010000);	//Write Device Address
	i2c_write(0b00000000);	//Init in dir 0h00
	i2c_stop();
	i2c_start();
	i2c_write(0b11010001);	//Read Devices Address
	*s= i2c_read(0);
	*min= i2c_read(0);
	*h= i2c_read(0);
	i2c_read(0);
	*d= i2c_read(0);
	*mes= i2c_read(0);
	*a= i2c_read(1);
	i2c_stop();
}