#include "peripherals.h"
#include "adafruit_control.h"
#include "witmotion_control.h"
#include "nrf_log_ctrl.h"
#include "app_error.h"
#include "nordic.h"
#include "Arduino.h"
#include "common.h"



extern "C" void main_loop_forever( void );

witmotion_data data = { 500.0 };

void main_loop_forever( void )
{

  for (;;)
  {
      if (NRF_LOG_PROCESS() == false)
      {
          uint32_t err_code = sd_app_evt_wait();
          APP_ERROR_CHECK(err_code);
      }
    
    if ( !wit_make_data_request() ) printf("Requested wit data\n");
    delay(500); 
    wit_read_data(&data);
    printf("a:%.2f %.2f %.2f\r\n",data.acceleration[0],data.acceleration[1],data.acceleration[2]);
    printf("w:%.2f %.2f %.2f\r\n",data.angular_velocity[0],data.angular_velocity[1],data.angular_velocity[2]);
    printf("Angle:%.1f %.1f %.1f\r\n",data.angle[0],data.angle[1],data.angle[2]);
  }
}


/**
 * @brief Function for application main entry.
 */
int main( void )
 {
    board_begin();

    // TODO: Move to `adafruit_init'
    pp_spi_init();
    adafruit_init();

    witmotion_init( 1 );
    main_loop_forever();
    return 0;
}


/**
 * @}
 */
