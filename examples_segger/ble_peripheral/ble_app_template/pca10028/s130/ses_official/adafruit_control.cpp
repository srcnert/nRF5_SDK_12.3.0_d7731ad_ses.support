/*
 * Interface to between Adafruit and main
*/
#include "adafruit_control.h"
#include "peripherals.h"
#include "common.h"
#include "PCD8544.h"



PCD8544 nokia = PCD8544(DISPLAY_CLK, DISPLAY_DIN, DISPLAY_DC, DISPLAY_CE, DISPLAY_RESET);


void adafruit_init( void  )
{
  adafruit_test();
  // nokia.init();
  // nokia.setContrast(40);
  // nokia.command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYALLON);
  // delay(500);
  // nokia.command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
  // nokia.display();
}

void adafruit_spiwrite (uint8_t c)
{
  spi_write(&c, 1);
}

void adafruit_longwrite (uint8_t *c, uint16_t len)
{
  spi_write(c, len);
}

void adafruit_print( char c )
{
  app_uart_put(c);
}
