#ifndef ADAFRUIT_CONTROL_H
#define ADAFRUIT_CONTROL_H

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

void adafruit_test( void );
void adafruit_init( void );
void adafruit_spiwrite (uint8_t c);
void adafruit_longwrite (uint8_t *c, uint16_t len);
void adafruit_print( char c );

#ifdef __cplusplus
}
#endif

#endif