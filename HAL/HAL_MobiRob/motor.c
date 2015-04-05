/*
 * motor.c
 *
 * Created: 05.03.2015 17:02:53
 *  Author: daniel.binggeli
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "HAL_MobiRob.h"
#include "io_port.h"

#define M1PLUS (1<<PC0)
#define M1MINUS (1<<PC1)
#define M2PLUS (1<<PC2)
#define M2MINUS (1<<PC3)
#define M3PLUS (1<<PC4)
#define M3MINUS (1<<PC5)
#define M4PLUS (1<<PC6)
#define M4MINUS (1<<PC7)
#define M1MASK (M1PLUS|M1MINUS)
#define M2MASK (M2PLUS|M2MINUS)
#define M3MASK (M3PLUS|M3MINUS)
#define M4MASK (M4PLUS|M4MINUS)

#define POWER_MAX 1024
#define POWER_MIN -1024
#define PWM_MAX 128

// Die Motoren 1 und 2 verwenden als Zeitgeber den Timer0, Motoren 3 und 4 den Timer2 
// Implementiert ist ein HW-PWM auf den Enable-Eingang der H-Brücke. 
// Die Timer laufen im Fast-PWM mit Tastfrequenz 20kHz und 8Bit Auflösung, d.i. 256 Stufen.
// Die Signale heissen M+, M- und PWM_M.
// Im Stillstand (power 0) werden die Anschlüsse M+, M- gegen Masse geschaltet, um einen Bremseffekt
// zu erzeugen.


uint8_t motor_init(void)
{
	// Initialisiere die verwendeten I/O-Ports als Ausgänge
	if(USE_MOTOR_1) // Motor1: M-, M+ und PWM_M
	{
		HAL_ddr_set(C, DDRC|M1MASK); 
		HAL_ddr_set(B, DDRB|(1<<PB7));
	}
 	if(USE_MOTOR_2) // Motor2: M-, M+ und PWM_M
	{
		HAL_ddr_set(C, DDRC|M2MASK); 
		HAL_ddr_set(G, DDRG|(1<<PG5));
	} 
	if(USE_MOTOR_3) // Motor3: M-, M+ und PWM_M
	{ 	HAL_ddr_set(C, DDRC|M3MASK); 
		HAL_ddr_set(B, DDRB|(1<<PB4));
	}
	if(USE_MOTOR_4) // Motor4: M-, M+ und PWM_M 
	{ 
		HAL_ddr_set(C, DDRC|M4MASK); 
		HAL_ddr_set(H, DDRH|(1<<PH6));
	}
		
	// Initialisiere die benötigten Timer zur Erzeugung der PWM
	// Benützt wird WGM Mode 3, Fast PWM mit TOP = 0xFF
	// Timer laufen mit ~1kHz an 16MHz Takt. Bei einem Prescaler von 1/64.
	// Die OCxy werden bei Bottom gesetzt, bei Match gelöscht, d.h. COMA = COMB = 2
	if(USE_MOTOR_1 | USE_MOTOR_2)
	{
		TCNT0 = 0;
		TCCR0A = 0x03; // WGM0 = 3
		if(USE_MOTOR_1)
		{
			OCR0A = 128;
			TCCR0A |= 0x80; // COM0A = 2
		}
		if(USE_MOTOR_2)
		{
			OCR0B = 128;
			TCCR0A |=  0x20; 	// COM0B = 2
		}
		TCCR0B = 0x02; // WGM0 = 3, CS0 = 2
	}
	if(USE_MOTOR_3 | USE_MOTOR_4)
	{
		TCNT2 = 0;
		TCCR2A = 0x03; // WGM2 = 3
		if(USE_MOTOR_3)
		{
			OCR2A = 128;
			TCCR2A |= 0x80; // COM2A = 2
		}
		if(USE_MOTOR_4)
		{
			OCR2B = 128;
			TCCR2A |= 0x20; 	// COM2B = 2
		}
		TCCR2B = 0x02; // WGM2 = 3, CS2 = 2
	}
	return 0;
}


uint8_t motor_set(uint8_t motor, int16_t power)
{
	static int8_t plus = 0;
	
	if(power < 0) {power=-power; plus=-1;}
	else if (power == 0) {power=POWER_MAX-1; plus=0;} 
	else {plus=1;}
	
	if(power > POWER_MAX) return 0xFF;
	
	power =  power / (POWER_MAX/PWM_MAX);
	switch(motor)
	{
		case 1: if(USE_MOTOR_1)
		{
			OCR0A=power; 
			if(plus>0) { HAL_port_set(C, (PORTC&~M1MASK)|M1PLUS);}
			else if(plus==0) {HAL_port_set(C, PORTC&~M1MASK);}
			else { HAL_port_set(C, (PORTC&~M1MASK)|M1MINUS);}
		}
		else return 0xFF;
		break;
		case 2: if(USE_MOTOR_2)
		{
			OCR0B=power; 
			if(plus>0) { HAL_port_set(C, (PORTC&~M2MASK)|M2PLUS);}
			else if(plus==0) {HAL_port_set(C, PORTC&~M2MASK);}
			else { HAL_port_set(C, (PORTC&~M2MASK)|M2MINUS);}
		}
		else return 0xFF;
		break;
		case 3: if(USE_MOTOR_3)
		{ 
			OCR2A=power; 
			if(plus>0) { HAL_port_set(C, (PORTC&~M3MASK)|M3PLUS);}
			else if(plus==0) {HAL_port_set(C, PORTC&~M3MASK);}
			else { HAL_port_set(C, (PORTC&~M3MASK)|M3MINUS);}
		}
		else return 0xFF;
		break;
		case 4: if(USE_MOTOR_4)
		{ 
			OCR2B=power; 
			if(plus>0) { HAL_port_set(C, (PORTC&~M4MASK)|M4PLUS);}
			else if(plus==0) {HAL_port_set(C, PORTC&~M4MASK);}
			else { HAL_port_set(C, (PORTC&~M4MASK)|M4MINUS);}
		}
		else return 0xFF;
		break;
	default: return 0xFF; // falscher Wert
	}
	return 0;
}