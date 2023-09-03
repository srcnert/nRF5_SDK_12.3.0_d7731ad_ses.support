
#ifndef _WITMOTION_CONTROL_H
#define _WITMOTION_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#define WIT_MAX_PERIOD 50  // max frequency = 200Hz
#define WIT_TIME_TO_WAIT_FOR_DATA 100
#define WIT_DATA_REQUEST_TIMEOUT 200

typedef struct
{
  float acceleration[3];
  float angular_velocity[3];
  float angle[3];
} witmotion_data; 

enum wit_definition {
  WIT_REQ_SUCCESS = 0,
  WIT_DATA_READY,
  WIT_DATA_NOT_READY,
  WIT_REQ_TIMEOUT
};
  

void witmotion_init( int start_or_stop );
void wit_make_data_request( void );
int wit_check_data_ready( void );
witmotion_data *wit_read_data( void );
void show_stats( void );
void show_data( void );


#ifdef __cplusplus
}
#endif

#endif