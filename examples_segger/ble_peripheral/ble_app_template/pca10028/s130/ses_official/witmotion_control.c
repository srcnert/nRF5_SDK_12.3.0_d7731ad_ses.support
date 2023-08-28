#include "witmotion_control.h"
#include <wit_c_sdk.h>
#include <nrf_error.h>
#include <bsp_config.h>
#include <nrf_delay.h>
#include <app_uart.h>
#include <app_timer.h>
#include <common.h>
#include <Arduino.h>


#define ACC_UPDATE    0x01
#define GYRO_UPDATE   0x02
#define ANGLE_UPDATE  0x04
#define MAG_UPDATE    0x08
#define READ_UPDATE   0x80

#define MODBUS_ADDR   0x50

#define UART_BUFFER_SIZE 16

typedef struct
{
  char data_request;
  char sending_data;
  char s_cDataUpdate;
} witmotion_evt;


static volatile witmotion_evt wit_evt = { 0, 0, 0 }; 

static void transmit_data( uint8_t *p_data, uint32_t uiSize );
static void process_data( uint32_t uiReg, uint32_t uiRegNum );
static void uart_event_handler( app_uart_evt_t *p_event );
static void witmotion_periodic_polling( void * p_context );
static void pp_uart_init( void );


// APP_TIMER_DEF(witmotion_polling_id);


void witmotion_init( int start_or_stop )
{
  uint32_t err_code = 0;
  if (start_or_stop) {
    pp_uart_init();
    WitInit(WIT_PROTOCOL_MODBUS, MODBUS_ADDR);
    WitSerialWriteRegister(transmit_data);
    WitRegisterCallBack(process_data);
    WitDelayMsRegister(nrf_delay_ms);
    //err_code = app_timer_create(&witmotion_polling_id,
    //                        APP_TIMER_MODE_REPEATED,
    //                        witmotion_periodic_polling);
    //if ( err_code == NRF_SUCCESS ) {
    //  err_code = app_timer_start(witmotion_polling_id,
    //                               APP_TIMER_TICKS(WITMOTION_POLLING_INTERVAL,0),
    //                               NULL);
  }
}


int wit_make_data_request( void )
{
  if (!wit_evt.data_request ) {
    wit_evt.data_request = 1;
    wit_evt.sending_data = 1;
    wit_evt.s_cDataUpdate = 0;
    WitReadReg(AX, 12);
    return 0;
  }
  return -1;
}


int wit_check_data_ready( void )
{
  if ( wit_evt.data_request && wit_evt.s_cDataUpdate )
    return 0;
  
  return -1;
}

  
void wit_read_data( witmotion_data *data )
{
  for (int i=0; i<3; i++) { 
    data->acceleration[i] = (float)sReg[AX+i]/32768.0f*16.0f;
    data->angular_velocity[i] = (float)sReg[GX+i]/32768.0f*2000.0f;
    data->angle[i] = (float)sReg[Roll+i]/32768.0f*180.0f;
  }
  wit_evt.data_request = 0; // set to 1 on data request
}


static void transmit_data(uint8_t *p_data, uint32_t uiSize)
{
  uint32_t i;
  for ( i=0; i<uiSize; i++ ) {
    wit_evt.sending_data = 1; 
    app_uart_put( p_data[i] );
    /*
    FIXME: We could be stuck in an infinite loop here. Use a the
    expected response time of the module as a timeout 
    */
    while (wit_evt.sending_data); // set to 0 in uart event handle.
  }
#if DEBUG_MODE_PRINTING == 1
  printf("Sent %d bytes: ", uiSize);
  for ( i=0; i<uiSize; i++ ) printf("%d ",p_data[i]);
  printf("\n");
#endif
  app_uart_flush(); // no-op
}


static void process_data(uint32_t uiReg, uint32_t uiRegNum)
{
#ifdef DEBUG_MODE_PRINTING
  printf("process_data: uiReg|uiRegNum: %d|%d\n", uiReg, uiRegNum);
#endif
  wit_evt.s_cDataUpdate = 1;  // set to 0 when new data requested
}


static void pp_uart_init( void )
{
    uint32_t err_code;

    /* uart module weirdly does not configure rx pin as input */
    nrf_gpio_cfg_input(RX_PIN_NUMBER, NRF_GPIO_PIN_PULLUP);
    
    const app_uart_comm_params_t comm_params =
    {
        .rx_pin_no = RX_PIN_NUMBER, // p024
        .tx_pin_no = TX_PIN_NUMBER, // p022
        .cts_pin_no = CTS_PIN_NUMBER,
        .rts_pin_no = RTS_PIN_NUMBER,
        .flow_control = APP_UART_FLOW_CONTROL_DISABLED,
        .use_parity = false,
        .baud_rate = UART_BAUDRATE_BAUDRATE_Baud9600
    };
    APP_UART_FIFO_INIT(&comm_params,
                      UART_BUFFER_SIZE,
                      UART_BUFFER_SIZE,
                      uart_event_handler,
                      APP_IRQ_PRIORITY_LOWEST,
                      err_code);
    //APP_UART_INIT(&comm_params,
    //               uart_event_handler,
    //               APP_IRQ_PRIORITY_LOWEST,
    //               err_code);
    APP_ERROR_CHECK(err_code);
#ifdef DEBUG_MODE_PRINTING
      char *s = "Hello World!\n\0";
      for (int i=0;i<strlen(s);i++) { app_uart_put(s[i]); delay(50); }
#endif
}


void uart_event_handler( app_uart_evt_t *p_event )
{
    static uint8_t c = 0;
    switch (p_event->evt_type)
    {
        case APP_UART_DATA_READY:
          if ( app_uart_get(&c) == NRF_SUCCESS ) WitSerialDataIn(c);
          break;
        case APP_UART_FIFO_ERROR:
          wit_evt.sending_data = 0;
#if DEBUG_MODE_PRINTING == 1
          printf("uart communication error!\n");
#endif
        case APP_UART_COMMUNICATION_ERROR:
            wit_evt.sending_data = 0;
            digitalWrite(LED_RGB_BLUE, LOW);
            break;
        case APP_UART_TX_EMPTY:
          wit_evt.sending_data = 0;
          break;
        case APP_UART_DATA:
        default:
            break;
    }
}