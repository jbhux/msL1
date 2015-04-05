/*
 * IncFile1.h
 *
 * Created: 05.03.2015 00:15:59
 *  Author: daniel.binggeli
 */ 


#ifndef IO_PORT_H_
#define IO_PORT_H_

#include <avr/io.h>
#include "HAL_config.h"

#define PORTDIR_A 0xFF // Die PORTDIR_x Definitionen geben die Richtung für die vom HAL benützten oder sonstwie vordefinierten Pins vor.
#define PORTDIR_B 0x96 // ACHTUNG: Änderungen an diesen Definitionen können die HAL-Funktionalität beeinträchtigen  
#define PORTDIR_C 0xFF // Der richtige Weg zum Freigeben von Teilen des HAL-Funktionsspektrums geht über die USE_xy-Flags 
#define PORTDIR_D 0xFC
#define PORTDIR_E 0x0A
#define PORTDIR_F 0x00
#define PORTDIR_G 0x24
#define PORTDIR_H 0x42
#define PORTDIR_J 0x40
#define PORTDIR_K 0x00
#define PORTDIR_L 0xC0

#define PORTMSK_A (0x00 | (USE_SERVO_0<<PA0)|(USE_SERVO_1<<PA1)|(USE_SERVO_2<<PA2)|(USE_SERVO_3<<PA3)|(USE_SERVO_4<<PA4)|(USE_SERVO_5<<PA5)|(USE_SERVO_6<<PA6)|(USE_SERVO_7<<PA7))
#define PORTMSK_B (0x00 | (USE_SPI?0x0F:0x00)|(USE_MOTOR_3<<PB4)|(USE_MOTOR_1<<PB7))
#define PORTMSK_C (0x00 | (USE_MOTOR_1?0x03:0x00)|(USE_MOTOR_2?0x0C:0x00)|(USE_MOTOR_3?0x30:0x00)|(USE_MOTOR_4?0xC0:0x00))
#define PORTMSK_D (0xFF) // Von I2C und LCD belegt
#define PORTMSK_E (0x20 | (USE_UART0?0x03:0x00)|(USE_SONIC_1?0x50:0x00)|(USE_SONIC_2?0x48:0x00)) // PE5 von USB Suspend vorbelegt
#define PORTMSK_F (0xF0 | (USE_IR_1<<PF0)|(USE_IR_2<<PF1)|(USE_IR_3<<PF2)|(USE_IR_4<<PF3)) // PF4:7 von JTAG vorbelegt
#define PORTMSK_G (0x00 | (USE_SERVO_8<<PG2)|(USE_MOTOR_2<<PG5))
#define PORTMSK_H (0x00 | (USE_UART2?0x03:0x00)|(USE_MOTOR_4<<PH6))
#define PORTMSK_J (0x03 | (USE_QUADDEC_2<<PJ2)|(USE_QUADDEC_1<<PJ3)|(USE_ENC_2<<PJ4)|(USE_ENC_1<<PJ5)|(USE_SERVO_9<<PJ6)) // PJ0:1 von USB vorbelegt
#define PORTMSK_K (0x01 | (USE_MAG_1<<PK1)|(USE_LINE_1<<PK2)|(USE_LINE_2<<PK3)|(USE_LINE_3<<PK4)|(USE_LINE_4<<PK5)) // PK0: von U-Batt vorbelegt
#define PORTMSK_L (0xFF) // Von LCD und Tastatur belegt

// 1 bedeutet Ausgang. 
// 0 bedeutet Eingang.
extern volatile uint8_t io_portA;
extern volatile uint8_t io_portB;
extern volatile uint8_t io_portC;
extern volatile uint8_t io_portD;
extern volatile uint8_t io_portE;
extern volatile uint8_t io_portF;
extern volatile uint8_t io_portG;
extern volatile uint8_t io_portH;
extern volatile uint8_t io_portJ;
extern volatile uint8_t io_portK;
extern volatile uint8_t io_portL;


// maskiert die vom HAL verwendeten Port-Bits weg
// 1 bedeutet belegt durch HAL. 
// 0 bedeutet frei für io_port-Verwendung
extern volatile uint8_t io_maskA;
extern volatile uint8_t io_maskB;
extern volatile uint8_t io_maskC;
extern volatile uint8_t io_maskD;
extern volatile uint8_t io_maskE;
extern volatile uint8_t io_maskF;
extern volatile uint8_t io_maskG;
extern volatile uint8_t io_maskH;
extern volatile uint8_t io_maskJ;
extern volatile uint8_t io_maskK;
extern volatile uint8_t io_maskL;

extern uint8_t port_init(void);

// Die folgenden drei Funktionen erlauben das selektive Lesen und Schreiben von Ports (PINx, PORTx, DDRx)
// "Selektiv" bedeutet hier: unter Anwendung einer Maske. Es werden nur Bits verändert, welche in der
// Maske eine eins aufweisen. Die anderen bleiben unverändert. Beim Lesen erscheinen die nicht durch 
// die Maske erfassten Bits als 0.
// HINWEIS: Dies sind Makros. Sie werden inline übersetzt und nicht durch Aufrufe eingebunden.
// VORSICHT: Diese Makros sind nicht für die Anwendung auf Applikationsebene vorgesehen. Verwenden
// Sie stattdessen die Funktionen ddr_set, port_set und port_get
/**
// set DDRx with mask(8-bit) to pattern (8-bit)
#define io_ddr_set(P, mask, pattern) (DDR##P=(((pattern)&(mask))|(DDR##P))|((DDR##P)&~(mask)))
// set PORTx with mask(8-bit) to pattern (8-bit)
#define	io_port_set(P, mask, pattern) ((PORT ## P) = (((PORT ## P) & (~((uint8_t)(mask)))) | \
	((uint8_t)(pattern) & (uint8_t)(mask))))
// get PINx with mask(8-bit), returns pattern (8-bit)
#define io_port_get(P, mask) (PIN ## P & (uint8_t)(mask))
**/

// Die folgenden drei Funktionen erlauben das sichere Lesen und Schreiben von Ports (PINx, PORTx, DDRx)
// "Sicher" bedeutet hier: ohne die HAL-Funktionalitäten zu tangieren
// HINWEIS: Dies sind Makros. Sie werden inline übersetzt und nicht durch Aufrufe eingebunden.

// ddr_set erlaubt dem Applikationsprogrammierer das 'sorglose' Schreiben auf jedes DDRx Register.
// Alle Bits, welche vom HAL benötigt werden, werden vor dem Schreiben wegmaskiert, so dass sie
// unverändert bleiben
#define ddr_set(P, pattern) (DDR##P=(((pattern)&~PORTMSK_##P)|(DDR##P&PORTMSK_##P))) 
// port_set erlaubt dem Applikationsprogrammierer das 'sorglose' Schreiben auf jedes PORTx Register.
// Alle Bits, welche vom HAL benötigt werden, werden vor dem Schreiben wegmaskiert, so dass sie
// unverändert bleiben
#define port_set(P, pattern) (PORT##P=(((pattern)&~PORTMSK_##P)|(PORT##P&PORTMSK_##P))) 
// port_get erlaubt dem Applikatiosnprogrammierer das 'sorglose' Lesen von PINx Registern.
// Alle Bits, welchevom HAL belegt sind, werden nach dem Lesen wegmaskiert, so dass sie
// als 0 erscheinen
#define port_get(P) ((PIN##P)&~(PORTMSK_##P))


// VORSICHT: Diese folgenden Makros sind nur für die Anwendung auf HAL-Ebene vorgesehen. Verwenden
// Sie stattdessen die Funktionen ddr_set, port_set und port_get

// HAL_ddr_set erlaubt dem HAL-Programmierer das 'sorglose' Schreiben auf jedes DDRx Register.
// Alle Bits, welche NICHT vom HAL benötigt werden, werden vor dem Schreiben wegmaskiert, so dass sie
// unverändert bleiben
#define HAL_ddr_set(P, pattern) (DDR##P=(((pattern)&PORTMSK_##P)|(DDR##P&~PORTMSK_##P)))
// HAL_port_set erlaubt dem HAL-Programmierer das 'sorglose' Schreiben auf jedes PORTx Register.
// Alle Bits, welche NICHT vom HAL benötigt werden, werden vor dem Schreiben wegmaskiert, so dass sie
// unverändert bleiben
#define HAL_port_set(P, pattern) (PORT##P=(((pattern)&PORTMSK_##P)|(PORT##P&~PORTMSK_##P)))
// HAL_port_get erlaubt dem HAL-Programmierer das 'sorglose' Lesen von PINx Registern.
// Alle Bits, welche NICHT vom HAL belegt sind, werden nach dem Lesen wegmaskiert, so dass sie
// als 0 erscheinen
#define HAL_port_get(P) ((PIN##P)&(PORTMSK_##P))

#endif /* IO_PORT_H_ */


