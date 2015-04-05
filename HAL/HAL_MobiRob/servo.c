/*
 * servo.c
 *
 * Created: 04.03.2015 22:09:48
 *  Author: daniel.binggeli
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "HAL_MobiRob.h"
#include "io_port.h"

// Die Servos verwenden als Zeitgeber den Timer5
// Implementiert ist ein Soft-PWM basierend auf zwei Interrupts:
// TIMER5_COMPA_vect und TIMER5_COMPB_vect.
// Der Servopuls ist 1..2ms (= 0%..100%)
// Es werden 10 Servos bedient, d.h. die Zykluszeit wird 20ms.

#define MAX_SERVOS (10)
#define PWM_0 (16000)
#define PWM_100 (32000)
#define PWM_SLOT (100)
#define POS_MAX (1000)

#define SERVO_MASK ((USE_SERVO_0?1:0)|(USE_SERVO_1?2:0)|(USE_SERVO_2?4:0)|(USE_SERVO_3?8:0)|\
(USE_SERVO_4?16:0)|(USE_SERVO_5?32:0)|(USE_SERVO_6?64:0)|(USE_SERVO_7?128:0)|(USE_SERVO_8?256:0)|\
(USE_SERVO_8?512:0))
volatile uint16_t servo_pwm[MAX_SERVOS];
volatile uint8_t servo = 0;

uint8_t servo_init(void)
{	
	// Initialisiere die verwendeten I/O-Ports als Ausgänge
	HAL_ddr_set(A, SERVO_MASK);
	if(USE_SERVO_8) HAL_ddr_set(G,(1<<PG2));
	if(USE_SERVO_9) HAL_ddr_set(J,(1<<PJ6));

	// initialisiere servo tabelle mit Mittelstellung
	for(servo = 0; servo < 10; servo++) servo_set(servo, 500);

	// Setze das betroffene IO-Port, wenn Servo 0 benützt wird
	servo = 0;
	if(USE_SERVO_0) HAL_port_set(A, (~(SERVO_MASK) | (1<<0)));
	
	 
	// Starte Timer5 PWM
	//// Timer 5 läuft im CTC-Modus (WGM5=4) mit 2ms Periodendauer
	// Timer 5 läuft im Fast-PWM-Modus (WGM5=15) mit 2ms Periodendauer
	// OCR5A dient als TOP, OCR5B wird für (Soft-)PWM verwendet
	// TIMER5_COMPA_vect und TIMER5_COMPB_vect Interrupts sind aktiv
	// Der Prescaler ist 1(CS5=1), 16-Bit Periodendauer wäre demnach 65536/16MHz = 4.096ms
	// für 2ms zählt der Zähler auf 32000.
	// WGM5 = 4; COM5x = 0; CS5 = 1;
	TCNT5 = 0;
	OCR5A = PWM_100+PWM_SLOT;
	OCR5B = servo_pwm[servo];
	TIFR5 = 0xFF; // alle Interrupt Flags löschen (AVR Spezial!)
	TIMSK5 = (1<<OCIE5A)|(1<<OCIE5B);
	TCCR5C = 0x00;
	TCCR5A = 0x03;
	TCCR5B = 0x19; // startet den Timer

	return 0;
}

uint8_t servo_set(uint8_t servo, uint16_t pos)
{
	if(servo >= MAX_SERVOS) return 0xFF; // falscher Wert
	if(pos > POS_MAX) return 0xFF; // dito
	
	// Berechne den PWM-Register Wert
	pos =  PWM_0 + ((PWM_100-PWM_0)/POS_MAX)*pos;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		servo_pwm[servo] = pos;
	}
	return 0;
}

ISR(TIMER5_COMPA_vect)
{
	// setze (nächsten) Servo Kanal auf 1, falls benützt
	switch(servo)
	{
	case 0: if(USE_SERVO_0) HAL_port_set(A, (1<<PA0));
		break;
	case 1: if(USE_SERVO_1) HAL_port_set(A, (1<<PA1));
		break;
	case 2: if(USE_SERVO_2) HAL_port_set(A, (1<<PA2));
		break;
	case 3: if(USE_SERVO_3) HAL_port_set(A, (1<<PA3));
		break;
	case 4: if(USE_SERVO_4) HAL_port_set(A, (1<<PA4));
		break;
	case 5: if(USE_SERVO_5) HAL_port_set(A, (1<<PA5));
		break;										
	case 6: if(USE_SERVO_6) HAL_port_set(A, (1<<PA6));
		break;										
	case 7: if(USE_SERVO_7) HAL_port_set(A, (1<<PA7));
		break;										
	case 8: if(USE_SERVO_8) HAL_port_set(G, (1<<PG2));
		break;
	case 9: if(USE_SERVO_9) HAL_port_set(J, (1<<PJ6));
		break;

	}
}

ISR(TIMER5_COMPB_vect)
{
	// reset alle benützten Servo-Kanal auf 0
	HAL_port_set(A, ~SERVO_MASK);
	if(USE_SERVO_8) HAL_port_set(G, ~(1<<PG2));
	if(USE_SERVO_9) HAL_port_set(J, ~(1<<PJ6));
	
	// wechsle zu nächstem Servo-Kanal
	++servo;
	if(servo >= MAX_SERVOS) servo = 0;
	// und setze schon mal das Ende des nächsten Pulses fest
	// die OCR Register werden in WGM=15 Modus erst bei BOTTOM
	// üernommen
	OCR5B = servo_pwm[servo];
}
