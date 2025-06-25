#ifndef RTC_H
#define RTC_H

#include "main.h"

void set_time(uint8_t s, uint8_t min, uint8_t h, uint8_t d, uint8_t mes, uint8_t a);
void get_time(uint8_t *s, uint8_t *min, uint8_t *h, uint8_t *d, uint8_t *mes, uint8_t *a);

#endif
