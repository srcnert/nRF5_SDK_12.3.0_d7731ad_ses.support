#ifndef ODOMETRE_H
#define ODOMETRE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <stdint.h>
#include <stddef.h>

void spi_write( uint8_t *buffer, size_t size);
void pp_spi_init( void );
void pp_uart_init( void );


#ifdef __cplusplus
}
#endif

#endif // ODOMETRE_H