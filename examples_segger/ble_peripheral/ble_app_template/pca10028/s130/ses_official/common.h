#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bsp.h>

#define DEBUG_MODE_PRINTING 1

#define DISPLAY_SPI_INSTANCE  0 /**< SPI instance index. */
#define SPI0_INSTANCE_INDEX 0
#define SPI1_INSTANCE_INDEX SPI0_INSTANCE_INDEX+SPI0_ENABLED

#define DISPLAY_BACKLIGHT_OFF bsp_board_led_on(bsp_board_pin_to_led_idx(DISPLAY_BACKLIGHT))
#define DISPLAY_BACKLIGHT_ON bsp_board_led_off(bsp_board_pin_to_led_idx(DISPLAY_BACKLIGHT))

#define DISPLAY_TX_BUF_LENGTH ((84 * 48) / 8) /**< SPI transaction buffer length. */


#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256                         /**< UART RX buffer size. */

typedef enum {
    DISPLAY_RESET = 19,
    DISPLAY_CE = 20,
    DISPLAY_DC = 14,
    DISPLAY_DIN = 10,
    DISPLAY_CLK = 6,
    DISPLAY_BACKLIGHT = LED_DISPLAY_BACKLIGHT
} DISPLAY_pin_t;

#ifdef __cplusplus
}
#endif

#endif // COMMON_H