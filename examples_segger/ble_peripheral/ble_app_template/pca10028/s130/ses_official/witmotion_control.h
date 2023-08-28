
#ifndef _WITMOTION_CONTROL_H
#define _WITMOTION_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#define WITMOTION_POLLING_INTERVAL 100

typedef struct
{
  float acceleration[3];
  float angular_velocity[3];
  float angle[3];
} witmotion_data; 

void witmotion_init( int start_or_stop );
int wit_make_data_request( void );
int wit_check_data_ready( void );
void wit_read_data( witmotion_data *data );


#ifdef __cplusplus
}
#endif

#endif