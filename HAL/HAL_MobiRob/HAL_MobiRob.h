/*
 * HAL_MobiRob.h
 *
 * Created: 04.03.2015 18:24:57
 *  Author: Daniel
 */ 
#ifndef HAL_MOBIROB_H_
#define HAL_MOBIROB_H_

#include <avr/io.h>
#include <stdbool.h>

#include "HAL_config.h"
#include "HAL_check.h"
#include "io_port.h"
#include "lcd.h"
#include "lcd_ext.h"
#include "encoder.h"

// DC_Motoren (1..4)
// #: PWM:     M+:  M-
// 1: OC0B/PG5 PC0  PC1
// 2: OC0A/PB7 PC2  PC3
// 3: OC2A/PB4 PC4  PC5
// 4: OC2B/PH6 PC6  PC7

// initialise motor resources
uint8_t motor_init(void);
// set motor (1..4) to power (-1000..1000)
uint8_t motor_set(uint8_t motor, int16_t power);

// Servos(0..9)
// 0..7:PA0..PA7
// 8, 9:PG2, PJ6

// initialise servo resources
uint8_t servo_init(void);
// set servo (0..9) to pos (0..1000)
uint8_t servo_set(uint8_t servo, uint16_t pos);

// Ultraschall(1..2)
// #  TRIG     ECHO
// 1: OC3B/PE4 INT7/PE7
// 2: OC3A/PE3 INT6/PE6

// Liniensensoren(1..4)
				   
// IR-Sensoren(1..4)

// Encoder

int32_t enc_get(uint8_t enc);
uint8_t enc_set(uint8_t enc, int32_t pos);
uint8_t direction_set(uint8_t enc, int8_t direction);
uint8_t enc_init(void);


// serial communications

// io's
// Hier kann die Funktion des System-Ticks und der Pin-Change Interrupts aktiviert werden.
// HINWEIS: Wenn ein Pin-Change auf einem der möglichen Pins Pxy überwacht werden soll, so muss das
// USE_PINCHANGE_Pxy-Flag als (1) definiert werden und zusätzlich muss die Funktion pinchange_Pxy
// geschrieben werden. Der Prototyp 'void pinchange_Pxy(uint8_t edge);' ist in HAL_MobiRob.h
// vordefiniert. 			 

// Vordefinierte Prototypes für die Call-Back Routinen
// HINWEIS: 
// Die Call-Back Routinen werden direkt von einer Interrupt-Routine aufgerufen. Das bringt als
// Vorteil eine sehr direkte, schnelle Reaktion auf die Pin-Change Ereignisse.
// Es müssen jedoch folgende Bedingungen und Einschränkungen beachtet werden:
// - Alle in Call-Back Routinen verwendeten Variablen müssen als 'volatile' deklariert werden.
// - Die Routinen sollen möglichst kurze Laufzeiten haben, KEINE Warteschlaufen.
// - Es sollen möglichst keine Funktionsaufrufe innerhalb von Call-Back Routinen aufgerufen werden.
// - Falls doch Funktionen aufgerufen werden, müssen diese als 'reentrant' deklariert worden sein,
//   und sollen ihrerseits sehr kurze Ausführungszeiten haben.
// - 


extern void pinchange_PB0(uint8_t edge);
extern void pinchange_PB1(uint8_t edge);
extern void pinchange_PB2(uint8_t edge);
extern void pinchange_PB3(uint8_t edge);
extern void pinchange_PB4(uint8_t edge);
extern void pinchange_PB5(uint8_t edge);
extern void pinchange_PB6(uint8_t edge);
extern void pinchange_PB7(uint8_t edge);
extern void pinchange_PE0(uint8_t edge);
extern void pinchange_PJ0(uint8_t edge);
extern void pinchange_PJ1(uint8_t edge);
extern void pinchange_PJ2(uint8_t edge);
extern void pinchange_PJ3(uint8_t edge);
extern void pinchange_PJ4(uint8_t edge);
extern void pinchange_PJ5(uint8_t edge);
extern void pinchange_PJ6(uint8_t edge);
extern void pinchange_PK0(uint8_t edge);
extern void pinchange_PK1(uint8_t edge);
extern void pinchange_PK2(uint8_t edge);
extern void pinchange_PK3(uint8_t edge);
extern void pinchange_PK4(uint8_t edge);
extern void pinchange_PK5(uint8_t edge);
extern void pinchange_PK6(uint8_t edge);
extern void pinchange_PK7(uint8_t edge);

// system
//
// initialise system timer, tick and  time
uint8_t	system_timer_init(void);

// get system time (0..2^32) [ms]
uint32_t time_get(void);

// get system tick
// returns true if  tick interval elapsed.
// Every call of tick_get() resets the tick flag
uint8_t	tick_get(void);

// system_tick ist eine Call-Back Routine
// Ihre Verwendung wird durch USE_SYSTEM_TICK gesteuert
void system_tick(uint32_t time);


#define KEY_UP		(1<<0)
#define KEY_R		(1<<1)
#define KEY_L		(1<<2)
#define KEY_DN		(1<<3)
#define KEY_MINUS	(1<<4)
#define KEY_PLUS	(1<<5)

// Initialize keys.
void  key_init(void);

// Return code of a pressed key from buffer.
// (0 if none).
uint8_t  key_get(void);

// Clear key buffer.
void  key_clear(void);

// Return true if a key was pressed.
// key_check macht die Auswertung der Tasten. Ein Tastendruck wird als gültig erkannt, wenn eine Taste 
// 2x gedrückt und dann 2x losgelassen wurde.
uint8_t  key_check(void);

// General ADC-State Enumeration
enum
{
	// selectable ADC-References
	VREF,
	AVCC,
	REF_1V1,
	REF_2V56,
	
	// selectable ADC-Resolution
	ADC_10BIT,
	ADC_8BIT
};
enum admux_values
{
	// Alle möglichen Kanalwahlen. Legende
	// ADCx		- Beteiligter Kanal
	// SINGLE	- Einkanalige Messung gegen Masse
	// DIFF		- Differentialmessung zwischen zwei Kanälen
	// 1		- Verstärkung 1
	// 10		- Verstärkung 10
	// 200		- Verstärkung 200
	// VREF_1_1V- Messung der 1.1V Referenz
	// VREF_0V	- Messung der Masse
	// RESERVEDx- nicht benützen
	ADC0_SINGLE = 0,
	ADC1_SINGLE,
	ADC2_SINGLE,
	ADC3_SINGLE,
	ADC4_SINGLE,
	ADC5_SINGLE,
	ADC6_SINGLE,
	ADC7_SINGLE,
	ADC0_ADC0_DIFF_10,
	ADC1_ADC0_DIFF_10,
	ADC0_ADC0_DIFF_200,
	ADC1_ADC0_DIFF_200,
	ADC2_ADC2_DIFF_10,
	ADC3_ADC2_DIFF_10,
	ADC2_ADC2_DIFF_200,
	ADC3_ADC2_DIFF_200,
	ADC0_ADC1_DIFF_1,
	ADC1_ADC1_DIFF_1,
	ADC2_ADC1_DIFF_1,
	ADC3_ADC1_DIFF_1,
	ADC4_ADC1_DIFF_1,
	ADC5_ADC1_DIFF_1,
	ADC6_ADC1_DIFF_1,
	ADC7_ADC1_DIFF_1,
	ADC0_ADC2_DIFF_1,
	ADC1_ADC2_DIFF_1,
	ADC2_ADC2_DIFF_1,
	ADC3_ADC2_DIFF_1,
	ADC4_ADC2_DIFF_1,
	ADC5_ADC2_DIFF_1,
	VREF_1_1V,
	VREF_0V,
	ADC8_SINGLE = 0x20,
	ADC9_SINGLE,
	ADC10_SINGLE,
	ADC11_SINGLE,
	ADC12_SINGLE,
	ADC13_SINGLE,
	ADC14_SINGLE,
	ADC15_SINGLE,
	ADC8_ADC8_DIFF_10,
	ADC9_ADC8_DIFF_10,
	ADC8_ADC8_DIFF_200,
	ADC9_ADC8_DIFF_200,
	ADC10_ADC10_DIFF_10,
	ADC11_ADC10_DIFF_10,
	ADC10_ADC10_DIFF_200,
	ADC11_ADC10_DIFF_200,
	ADC8_ADC9_DIFF_1,
	ADC9_ADC9_DIFF_1,
	ADC10_ADC9_DIFF_1,
	ADC11_ADC9_DIFF_1,
	ADC12_ADC9_DIFF_1,
	ADC13_ADC9_DIFF_1,
	ADC14_ADC9_DIFF_1,
	ADC15_ADC9_DIFF_1,
	ADC8__ADC10_DIFF_1,
	ADC9__ADC10_DIFF_1,
	ADC10_ADC10_DIFF_1,
	ADC11_ADC10_DIFF_1,
	ADC12_ADC10_DIFF_1,
	ADC13_ADC10_DIFF_1,
	RESERVED1,
	RESERVED2,
};







// Do a single AD-Conversion
uint16_t adc_read(uint8_t adc_input);

// Initialize the AD-Converter
void adc_init(uint8_t adc_reference);
#endif /* HAL_MOBIROB_H_ */