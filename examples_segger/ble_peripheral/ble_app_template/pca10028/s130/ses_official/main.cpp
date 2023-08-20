#include "peripherals.h"
#include "adafruit_control.h"
#include "nrf_log_ctrl.h"
#include "app_error.h"
#include "nordic.h"
#include "Arduino.h"



extern "C" void main_loop_forever( void );


void main_loop_forever( void )
{
    // Enter main loop.
    //NRF_LOG_INIT(NULL);

    for (;; )
    {
        if (NRF_LOG_PROCESS() == false)
        {
            uint32_t err_code = sd_app_evt_wait();
            APP_ERROR_CHECK(err_code);
        }
    }
}


/**
 * @brief Function for application main entry.
 */
int main( void )
{
    board_begin();
    pp_uart_init();
    pp_spi_init();
    adafruit_init();
    main_loop_forever();
}


/**
 * @}
 */
