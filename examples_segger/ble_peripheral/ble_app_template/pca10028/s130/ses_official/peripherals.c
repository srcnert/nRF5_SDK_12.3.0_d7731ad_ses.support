#include <nrf_drv_spi.h>
#include <bsp.h>
#include <string.h>
#include "nrf_delay.h"
#include "app_uart.h"
#include "nrf_uart.h"
#include "nrf_log_ctrl.h"
#include "common.h"


static const nrf_drv_spi_t spi_object = NRF_DRV_SPI_INSTANCE(DISPLAY_SPI_INSTANCE);  /**< SPI instance. */
static volatile bool spi_xfer_done = false;  /**< Flag used to indicate that SPI instance completed the transfer. */



void spi_write( uint8_t *buffer, size_t size)
{
    spi_xfer_done = false;
    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi_object, buffer, size, NULL, 1));
    while (!spi_xfer_done)
        __WFE();
}

static void spi_event_handler(nrf_drv_spi_evt_t const * p_event)
{
    spi_xfer_done = true;
}

static void uart_event_handler(app_uart_evt_t *p_event)
{
    switch (p_event->evt_type)
    {
        case APP_UART_DATA_READY:
        case APP_UART_FIFO_ERROR:
        case APP_UART_COMMUNICATION_ERROR:
        case APP_UART_TX_EMPTY:
        case APP_UART_DATA:
            break;
        default:
            break;
    }
}

void pp_spi_init( void )
{
    DISPLAY_BACKLIGHT_ON;
    nrf_delay_ms(100);
    nrf_drv_spi_config_t DISPLAY_spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    DISPLAY_spi_config.ss_pin = DISPLAY_CE;
    DISPLAY_spi_config.mosi_pin = DISPLAY_DIN;
    DISPLAY_spi_config.sck_pin  = DISPLAY_CLK;
   //bsp_board_led_invert(bsp_board_pin_to_led_idx(DISPLAY_BACKLIGHT));
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi_object, &DISPLAY_spi_config, spi_event_handler));
    DISPLAY_BACKLIGHT_OFF;
}

void pp_uart_init(void)
{
    char *hello = "Hello World !!!\n\r\0";
    uint32_t err_code;
    const app_uart_comm_params_t comm_params =
    {
        .rx_pin_no = RX_PIN_NUMBER,
        .tx_pin_no = TX_PIN_NUMBER,
        .cts_pin_no = CTS_PIN_NUMBER,
        .rts_pin_no = RTS_PIN_NUMBER,
        .flow_control = APP_UART_FLOW_CONTROL_DISABLED,
        .use_parity = false,
        .baud_rate = UART_BAUDRATE_BAUDRATE_Baud9600
    };

    APP_UART_INIT(&comm_params,
                       uart_event_handler,
                       APP_IRQ_PRIORITY_LOWEST,
                       err_code);

    APP_ERROR_CHECK(err_code);

    for (int i = 0; i < strlen(hello); i++)
    {
        while (app_uart_put(hello[i]) != NRF_SUCCESS);
    }
}


