#ifndef ARDUINO_H
#define ARDUINO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <boards.h>
#include <nrf_delay.h>
#include "app_uart.h"


/*
 * Arduino uses active HIGH, nRF uses active LOW. So for a HIGH on nRF, we write
 * 'off', which will caise the pin to stop sinking current, taking it high because
 * of the pull-up resistor.
*/
#define HIGH 1
#define LOW 0
#define digitalWrite(pin, value) if (value) bsp_board_led_off(bsp_board_pin_to_led_idx(pin)); else \
    bsp_board_led_on(bsp_board_pin_to_led_idx(pin))


#define OUTPUT 1    // INPUT not used
#define pinMode(pin, mode) if (mode) { nrf_gpio_cfg_output(pin); }

#define delay(ms) nrf_delay_ms(ms)

#define _BV(bit) (1 << (bit))

#define pgm_read_byte(addr) (*(const uint8_t *)(addr))

#ifdef __cplusplus
}
#endif

#endif