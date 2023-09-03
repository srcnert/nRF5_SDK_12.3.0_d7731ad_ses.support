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

#define UART_BUFFER_SIZE 64

typedef struct
{
  char data_request;
  char sending_data;
  char s_cDataUpdate;
  char request_timeout;
  uint32_t total_requests;
  uint32_t total_reads;
  uint32_t total_timeouts;
} witmotion_evt;


static volatile witmotion_evt wit_evt = { 0 };
static witmotion_data wit_data = { 99.99 };

static void transmit_data( uint8_t *p_data, uint32_t uiSize );
static void process_data( uint32_t uiReg, uint32_t uiRegNum );
static void wit_uart_event_handler( app_uart_evt_t *p_event );
static void wit_request_timeout_handler( void * p_context );
static void pp_uart_init( void );


APP_TIMER_DEF(witmotion_polling_id);


void witmotion_init( int start_or_stop )
{
  if (start_or_stop) {
    pp_uart_init();
    WitInit(WIT_PROTOCOL_MODBUS, MODBUS_ADDR);
    WitSerialWriteRegister(transmit_data);
    WitRegisterCallBack(process_data);
    WitDelayMsRegister(nrf_delay_ms);
    //app_timer_create(&witmotion_polling_id,
    //                APP_TIMER_MODE_SINGLE_SHOT,
    //                wit_request_timeout_handler);
  }
}


void wit_make_data_request( void )
{
  wit_evt.data_request = 1;
  wit_evt.s_cDataUpdate = 0;
  wit_evt.total_requests += 1;
  wit_evt.request_timeout = 0;
  WitReadReg(AX, 12);
  //app_timer_start(witmotion_polling_id,
  //                            APP_TIMER_TICKS(WIT_DATA_REQUEST_TIMEOUT,0),
  //                            NULL);
  //
}


int wit_check_data_ready( void )
{
  if (wit_evt.s_cDataUpdate && wit_evt.data_request )
    return WIT_DATA_READY;

  if (wit_evt.request_timeout)
    return WIT_REQ_TIMEOUT;

  return WIT_DATA_NOT_READY;
}


void show_data( void )
{
  printf("a:%.2f %.2f %.2f\r\n",wit_data.acceleration[1],wit_data.acceleration[2],wit_data.acceleration[3]);
  printf("w:%.2f %.2f %.2f\r\n",wit_data.angular_velocity[1],wit_data.angular_velocity[2],wit_data.angular_velocity[3]);
  printf("<:%.1f %.1f %.1f\r\n",wit_data.angle[0],wit_data.angle[1],wit_data.angle[2]);
}


witmotion_data *wit_read_data( void )
{
  for (int i=0; i<3; i++) {
    wit_data.acceleration[i] = (float)sReg[AX+i]/32768.0f*16.0f;
    wit_data.angular_velocity[i] = (float)sReg[GX+i]/32768.0f*2000.0f;
    wit_data.angle[i] = (float)sReg[Roll+i]/32768.0f*180.0f;
  }
#if DEBUG_MODE_PRINTING
  // show_data();
#endif
  wit_evt.request_timeout = 0;
  wit_evt.s_cDataUpdate = 0;
  wit_evt.data_request = 0; // set to 1 on data request
  return &wit_data;
}


static void transmit_data(uint8_t *p_data, uint32_t uiSize)
{
  uint32_t i,err_code;
  wit_evt.sending_data = 1; // set to 0 in application uart event handler
  for ( i=0,err_code=0; i<uiSize; i++ ) app_uart_put(p_data[i]); // add to fifo
}


static void process_data(uint32_t uiReg, uint32_t uiRegNum)
{
  wit_evt.total_reads += 1;
  wit_evt.s_cDataUpdate = 1;  // set to 0 when new data requested
}


void show_stats( void )
{
  printf(
    "treq|tread|ttime: %i|%i|%i\r             ",
    wit_evt.total_requests,
    wit_evt.total_reads,
    wit_evt.total_timeouts);
}


static void pp_uart_init( void )
{
    uint32_t err_code;

    const app_uart_comm_params_t comm_params =
    {
        .rx_pin_no = RX_PIN_NUMBER, // p024
        .tx_pin_no = TX_PIN_NUMBER, // p022
        .cts_pin_no = CTS_PIN_NUMBER,
        .rts_pin_no = RTS_PIN_NUMBER,
        .flow_control = APP_UART_FLOW_CONTROL_DISABLED,
        .use_parity = false,
        .baud_rate = UART_BAUDRATE_BAUDRATE_Baud115200
    };

    /* uart module weirdly does not configure rx pin as input
       alternative way to enable pullup:
        NRF_GPIO->PIN_CNF[RX_PIN_NUMBER] |=3<<2 ;
    */
    nrf_gpio_cfg_input(RX_PIN_NUMBER, NRF_GPIO_PIN_PULLUP);

    APP_UART_FIFO_INIT(&comm_params,
                      UART_BUFFER_SIZE,
                      UART_BUFFER_SIZE,
                      wit_uart_event_handler,
                      APP_IRQ_PRIORITY_LOWEST,
                      err_code);
    //APP_UART_INIT(&comm_params,
    //               uart_event_handler,
    //               APP_IRQ_PRIORITY_LOWEST,
    //               err_code);
    APP_ERROR_CHECK(err_code);
}


static void wit_request_timeout_handler( void * p_context )
{
  wit_evt.request_timeout = 1;
  wit_evt.total_timeouts += 1;
}


static void wit_uart_event_handler( app_uart_evt_t *p_event )
{
    static uint8_t c = 0;
    uint32_t err_code;
    switch (p_event->evt_type)
    {
        case APP_UART_DATA_READY:
          err_code = app_uart_get(&c);
          WitSerialDataIn(c);
          // printf("rx: %c\n",c);
          break;
        case APP_UART_FIFO_ERROR:
          break;
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