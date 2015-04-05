/*
 * system.c
 *
 * Created: 25.03.2015 16:26:35
 *  Author: daniel
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/atomic.h>
#include "HAL_MobiRob.h"

volatile uint32_t system_time = 0UL;
volatile bool systick = false;

// Für Tastatur
volatile uint8_t keys;
uint8_t old_keys;
uint8_t keys_make;
uint8_t key_buffer;
// KEY_MASK: Alle Tasten die gescannt werden sollen
#define KEY_MASK (KEY_UP|KEY_R|KEY_L|KEY_DN|KEY_MINUS|KEY_PLUS)
// KEY_FLAG: Nicht benützte Bits des Tastencodes. Signalisieren: neuen Scan duch die ISR erfolgt
#define KEY_FLAG (~KEY_MASK)

// initialise system timer, tick and  time
uint8_t	system_timer_init(void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		systick = false;
		system_time = 0L;
		// Timer 3 im Mode 14 (Fast PWM (CTC) mit ICR3	: WGM3 = 1110
		// - TOV3 kommt bei TOP(=ICR3)
		// - PWMs sind ausgeschaltet					: COM3x = 00
		// - 
		// Prescaler: 1 (4ms)							: CS3 = 001
		// clkT3 16MHz
		// Reload 4ms: 16M*0.004 = 64000
		// Prescaler: 8 (8-32ms)						: CS3 = 010
		// clkT3 2MHz
		// Reload 8ms: 2M*0.008 = 16000
		// Reload 32ms: 2M*0.032 = 64000
		// Prescaler: 64 (36-256ms)						: CS3 = 011
		// clkT3 250kHz
		// Reload 36ms: 250k*0.036 = 9000
		// Reload 256ms: 250k*0.256 = 64000
		// Prescaler: 256 (260-1024ms)					: CS3 = 100
		// clkT3 62.5kHz
		// Reload 260ms: 62.5k*0.26 = 16250
		// Reload 1024ms: 62.5k*1.024 = 64000
		// 
#define WGM3_VAL (14)
#define COM3x_VAL (0)
#if SYSTEM_INTERVAL % 4
	#error: defined a wrong value for SYSTEM_INTERVAL. Should be a multiple of 4
#elif SYSTEM_INTERVAL > 1024
	#error: defined a wrong value for SYSTEM_INTERVAL. Should be <= 1024
#elif SYSTEM_INTERVAL > 256
	#define PRESCALER (256L)
	#define CS3_VAL (0x04)
#elif SYSTEM_INTERVAL > 32
	#define PRESCALER (64L)
	#define CS3_VAL (0x03)
#elif SYSTEM_INTERVAL > 4
	#define PRESCALER (8L)
	#define CS3_VAL (0x02)
#elif SYSTEM_INTERVAL == 4
	#define PRESCALER (1L)
	#define CS3_VAL (0x01)
#else
	#error: defined a wrong value for SYSTEM_INTERVAL. Should be > 0
#endif
#define RELOAD_VAL (((F_CPU/PRESCALER)*SYSTEM_INTERVAL)/1000)
	
	TCNT3 = 0;
	OCR3A = OCR3B = OCR3C = 0;
	TIFR3 = 0xFF;  // reset all interrupt flags
	TIMSK3 = (1<<TOIE3);
	ICR3 = RELOAD_VAL;
	TCCR3C = 0;
	TCCR3A = ((COM3x_VAL<<COM3A0)|(COM3x_VAL<<COM3B0)|(COM3x_VAL<<COM3C0)|(WGM3_VAL&(0b0011)));
	TCCR3B = (((WGM3_VAL>>2)<<WGM32)|CS3_VAL);
	}	
	return 0;
}

// get system time (0..2^32) [ms]
uint32_t time_get(void)
{
	uint32_t systime;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		systime = system_time;
	}
	return systime;
}

// get system time (0..2^42) [us]
uint64_t time_us_get(void)
{
	uint64_t ustime;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		// Timer T3 im Mikrosekunden 
		// je nach PRESCALER 1:(0..4095) 256:(0..1048320)
#if (PRESCALER > (F_CPU/1000000UL))
		ustime = TCNT3*(uint64_t)(PRESCALER/(F_CPU/1000000UL));
#else
		ustime = TCNT3/((uint64_t)(F_CPU/1000000UL)/PRESCALER);
#endif //PRESCALER
		// PLUS systemzeit in Millisekunden
		ustime += system_time*1000ULL;
	}
	return ustime;
}


// get system tick
// returns true if  tick interval elapsed.
// Every call of tick_get() resets the tick flag
uint8_t	tick_get(void)
{
	uint8_t tick;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		tick = systick;
		systick = false;
	}
	return tick;
}
//
void system_tick(uint32_t time)
{
	
}

ISR (TIMER3_OVF_vect)
{
	DDRG=(DDRG|0x03);
	PORTG=(PORTG|0x03);
	
	systick = true;
	system_time += (uint32_t)SYSTEM_INTERVAL;
	

	
	// Der System-Timer ruft in einem konfigurierbaren Intervall (4..1024 ms) diese Routine auf.
#if USE_SYSTEM_TICK==1	
	system_tick(system_time);
#endif
	keys=((~PINL&KEY_MASK)|KEY_FLAG);
	
	PORTG=(PORTG&~0x03);
}



// Initialize keys.
void  key_init (void)
{
	HAL_ddr_set(L, (DDRL&~KEY_MASK)); // Keys sind Eingänge
	HAL_port_set(L, KEY_MASK);	 // aber mit Pull-Ups
	keys=keys_make=old_keys=0;
	key_buffer=0;
	
}
// Return code of a pressed key from buffer.
// (0 if none).
uint8_t  key_get(void)
{
	uint8_t ret=key_buffer;
	key_clear();
	key_check();
	return ret;
}
// Clear key buffer.
inline void  key_clear(void)
{
	key_buffer=0;
}
// Return true if a key was pressed.
// key_check macht die Auswertung der Tasten. Ein Tastendruck wird als gültig erkannt, wenn sie 2x 
// gedrückt und dann 2x losgelassen wurde.
uint8_t  key_check(void)
{
	uint8_t k;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		k=keys; // atomic Zugriff auf keys, welches von der ISR verändert wird
		keys=k&KEY_MASK; // KEY_FLAG löschen, da keys gelesen
	}
	if(k&KEY_FLAG) //KEY_FLAGS signalisiert: ISR hat neuen Tastencode gescannt
	{
		k=(k&KEY_MASK); // Flag wegmaskieren
		if((k==0)&&(old_keys==0)) // Schon zum 2. mal keine Taste gedrückt
		{
			if(keys_make)        // Wenn eine Taste gedrückt war
			{
				key_buffer=keys_make;  // Tastencode puffern
				keys_make=0;
			}
		}
		else  // (mindestens) eine Taste gedrückt ...
		{
			if(k==old_keys) keys_make=k; // keys_make wird gesetzt wenn 2x dieselbe T gedrückt war
		}
		old_keys=k;
	}
	return (key_buffer!=0);	
}