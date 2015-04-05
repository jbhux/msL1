/*
 * Encoder.c
 *
 * Created: 19.03.2015 17:47:52
 *  Author: daniel
 */ 
#include <avr/io.h>
#include <util/atomic.h>
#include <limits.h>
#include "HAL_MobiRob.h"
#include "io_port.h"
#include "encoder.h"

volatile int32_t encoder1 =0L;
volatile int32_t encoder2 =0L;
volatile int8_t dir1 = 0;
volatile int8_t dir2 = 0;

#if USE_ENC_1 == 1
void pinchange_PJ5(uint8_t edge)
{
#if USE_QUADDEC_1 == 1
	if(edge) // rising edge of signal A
	{
		if(HAL_port_get(J)&(1<<PJ3)) --encoder1; // high level on signal B -> reverse
		else ++encoder1;						 // low level on signal B -> forward
	}
	else // falling edge on signal A
	{
		if(HAL_port_get(J)&(1<<PJ3)) ++encoder1; // high level on signal B -> forward
		else --encoder1;						 // low level on signal B -> reverse
	}
#else
	if(dir1>0) ++encoder1;			// forward direction selected
	else if (dir <0) --encoder1;	// reverse direction selected
	else ;							// no direction selected, no move
#endif
}
#endif //(USE_ENC_1)

#if USE_ENC_2 == 1
void pinchange_PJ4(uint8_t edge)
{
#if USE_QUADDEC_2 == 1
	if(edge) // rising edge of signal A
	{
		if(HAL_port_get(J)&(1<<PJ2)) --encoder2; // high level on signal B -> reverse
		else ++encoder2;						 // low level on signal B -> forward
	}
	else // falling edge on signal A
	{
		if(HAL_port_get(J)&(1<<PJ2)) ++encoder2; // high level on signal B -> forward
		else --encoder2;						 // low level on signal B -> reverse
	}
#else
	if(dir2>0) ++encoder2;			// forward direction selected
	else if (dir2<0) --encoder2;	// reverse direction selected
	else ;							// no direction selected, no move
#endif
}
#endif //(USE_ENC_2)

int32_t enc_get(uint8_t enc)
{
     int32_t enc_copy = LONG_MIN;
	 if(enc==1)
	 {
		 ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		 {
			 enc_copy=encoder1;
		 }
	 }
	 else if(enc==2)
	 {
		 ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		 {
			 enc_copy=encoder2;
		 }
	 }
	 return enc_copy;
}

uint8_t enc_set(uint8_t enc, int32_t pos)
{
	uint8_t retval = 0xFF; // ungültige Eingabe
	if(enc==1)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			encoder1=pos;
		}
		retval=0; // ok
	}
	else if(enc==2)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			encoder2=pos;
		}
		retval=0;
	}
	return retval; // ok
}

uint8_t direction_set(uint8_t enc, int8_t direction)
{
	uint8_t retval = 0xFF; // ungültige Eingabe
	if(enc==1)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			dir1=direction;
		}
		retval=0; // ok
	}
	else if(enc==2)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			dir2=direction;
		}
		retval=0;
	}
	return retval; // ok
}

uint8_t enc_init(void)
{
	// Pin-Change Interrupts werden in input.c schon initialisiert
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		encoder1=encoder2=0L;
		dir1=dir2=0;
	}
	return 0;	
}