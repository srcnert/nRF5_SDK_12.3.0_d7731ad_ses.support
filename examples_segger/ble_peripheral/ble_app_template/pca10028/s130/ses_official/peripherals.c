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


void pp_spi_init( void )
{
    DISPLAY_BACKLIGHT_ON;
    nrf_delay_ms(100);
    nrf_drv_spi_config_t DISPLAY_spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    DISPLAY_spi_config.ss_pin = DISPLAY_CE;
    DISPLAY_spi_config.mosi_pin = DISPLAY_DIN;
    DISPLAY_spi_config.sck_pin  = DISPLAY_CLK;
    nrf_gpio_cfg_output(DISPLAY_RESET);
    nrf_gpio_cfg_output(DISPLAY_DC);
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi_object, &DISPLAY_spi_config, spi_event_handler));
    //APP_ERROR_CHECK(nrf_drv_spi_init(&spi_object, &DISPLAY_spi_config, NULL));
    DISPLAY_BACKLIGHT_OFF;
}


