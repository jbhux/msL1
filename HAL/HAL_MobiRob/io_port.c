/*
 * io_port.c
 *
 * Created: 05.03.2015 09:56:27
 *  Author: daniel.binggeli
 */ 
#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "HAL_MobiRob.h"
#include "io_port.h"

// Gibt die Datenrichtung vor für die HAL-I/Os
// (DDR Wert für die vom HAL verwendeten IO-Ports)
// 1 bedeutet Ausgang.
// 0 bedeutet Eingang.
/***volatile uint8_t io_portA = PORTDIR_A;
volatile uint8_t io_portB = PORTDIR_B;
volatile uint8_t io_portC = PORTDIR_C;
volatile uint8_t io_portD = PORTDIR_D;
volatile uint8_t io_portE = PORTDIR_E;
volatile uint8_t io_portF = PORTDIR_F;
volatile uint8_t io_portG = PORTDIR_G;
volatile uint8_t io_portH = PORTDIR_H;
volatile uint8_t io_portJ = PORTDIR_J;
volatile uint8_t io_portK = PORTDIR_K;
volatile uint8_t io_portL = PORTDIR_L;


// maskiert die vom HAL verwendeten Port-Bits weg
// 1 bedeutet belegt durch HAL.
// 0 bedeutet frei für io_port-Verwendung
volatile uint8_t io_maskA = PORTMSK_A;
volatile uint8_t io_maskB = PORTMSK_B;
volatile uint8_t io_maskC = PORTMSK_C;
volatile uint8_t io_maskD = PORTMSK_D;
volatile uint8_t io_maskE = PORTMSK_E;
volatile uint8_t io_maskF = PORTMSK_F;
volatile uint8_t io_maskG = PORTMSK_G;
volatile uint8_t io_maskH = PORTMSK_H;
volatile uint8_t io_maskJ = PORTMSK_J;
volatile uint8_t io_maskK = PORTMSK_K;
volatile uint8_t io_maskL = PORTMSK_L;



****/
#define NUM_CBR 24

struct CBR {
	bool valid;
	void (*ptr)(uint8_t edge);
};

volatile struct CBR cbr[ NUM_CBR] =
{
	{.valid=USE_PINCHANGE_PB0, 	.ptr=(USE_PINCHANGE_PB0?pinchange_PB0:NULL)}, //PB0,PCINT0				   
	{.valid=USE_PINCHANGE_PB1, 	.ptr=(USE_PINCHANGE_PB1?pinchange_PB1:NULL)}, //PB1,PCINT1				   
	{.valid=USE_PINCHANGE_PB2, 	.ptr=(USE_PINCHANGE_PB2?pinchange_PB2:NULL)}, //PB2,PCINT2				   
	{.valid=USE_PINCHANGE_PB3, 	.ptr=(USE_PINCHANGE_PB3?pinchange_PB3:NULL)}, //PB3,PCINT3				   
	{.valid=USE_PINCHANGE_PB4, 	.ptr=(USE_PINCHANGE_PB4?pinchange_PB4:NULL)}, //PB4,PCINT4				   
	{.valid=USE_PINCHANGE_PB5, 	.ptr=(USE_PINCHANGE_PB5?pinchange_PB5:NULL)}, //PB5,PCINT5				   
	{.valid=USE_PINCHANGE_PB6, 	.ptr=(USE_PINCHANGE_PB6?pinchange_PB6:NULL)}, //PB6,PCINT6				   
	{.valid=USE_PINCHANGE_PB7, 	.ptr=(USE_PINCHANGE_PB7?pinchange_PB7:NULL)}, //PB7,PCINT7				   
	{.valid=USE_PINCHANGE_PE0, 	.ptr=(USE_PINCHANGE_PE0?pinchange_PE0:NULL)}, //PE0,PCINT8				   
	{.valid=USE_PINCHANGE_PJ0, 	.ptr=(USE_PINCHANGE_PJ0?pinchange_PJ0:NULL)}, //PJ0,PCINT9				   
	{.valid=USE_PINCHANGE_PJ1, 	.ptr=(USE_PINCHANGE_PJ1?pinchange_PJ1:NULL)}, //PJ1,PCINT10				   
	{.valid=USE_PINCHANGE_PJ2, 	.ptr=(USE_PINCHANGE_PJ2?pinchange_PJ2:NULL)}, //PJ2,PCINT11				   
	{.valid=USE_PINCHANGE_PJ3, 	.ptr=(USE_PINCHANGE_PJ3?pinchange_PJ3:NULL)}, //PJ3,PCINT12				   
	{.valid=USE_PINCHANGE_PJ4, 	.ptr=(USE_PINCHANGE_PJ4?pinchange_PJ4:NULL)}, //PJ4,PCINT13				   
	{.valid=USE_PINCHANGE_PJ5, 	.ptr=(USE_PINCHANGE_PJ5?pinchange_PJ5:NULL)}, //PJ5,PCINT14				   
	{.valid=USE_PINCHANGE_PJ6, 	.ptr=(USE_PINCHANGE_PJ6?pinchange_PJ6:NULL)}, //PJ6,PCINT15				   
	{.valid=USE_PINCHANGE_PK0, 	.ptr=(USE_PINCHANGE_PK0?pinchange_PK0:NULL)}, //PK0,PCINT16				   
	{.valid=USE_PINCHANGE_PK1, 	.ptr=(USE_PINCHANGE_PK1?pinchange_PK1:NULL)}, //PK1,PCINT17				   
	{.valid=USE_PINCHANGE_PK2, 	.ptr=(USE_PINCHANGE_PK2?pinchange_PK2:NULL)}, //PK2,PCINT18				   
	{.valid=USE_PINCHANGE_PK3, 	.ptr=(USE_PINCHANGE_PK3?pinchange_PK3:NULL)}, //PK3,PCINT19				   
	{.valid=USE_PINCHANGE_PK4, 	.ptr=(USE_PINCHANGE_PK4?pinchange_PK4:NULL)}, //PK4,PCINT20				   
	{.valid=USE_PINCHANGE_PK5, 	.ptr=(USE_PINCHANGE_PK5?pinchange_PK5:NULL)}, //PK5,PCINT21				   
	{.valid=USE_PINCHANGE_PK6, 	.ptr=(USE_PINCHANGE_PK6?pinchange_PK6:NULL)}, //PK6,PCINT22				   
	{.valid=USE_PINCHANGE_PK7, 	.ptr=(USE_PINCHANGE_PK7?pinchange_PK7:NULL)}, //PK7,PCINT23				   
};

volatile uint8_t shadowB = 0;
volatile uint8_t maskB = 0;
volatile uint8_t shadowEJ = 0;
volatile uint8_t maskEJ = 0;
volatile uint8_t shadowK = 0;
volatile uint8_t maskK = 0;

uint8_t port_init(void)
{
	// normale IO-Ports DD-Register
	DDRA = ((PORTMSK_A & PORTDIR_A) | (DDRA & ~PORTMSK_A));
	DDRB = ((PORTMSK_B & PORTDIR_B) | (DDRB & ~PORTMSK_B));
	DDRC = ((PORTMSK_C & PORTDIR_C) | (DDRC & ~PORTMSK_C));
	DDRD = ((PORTMSK_D & PORTDIR_D) | (DDRD & ~PORTMSK_D));
	DDRE = ((PORTMSK_E & PORTDIR_E) | (DDRE & ~PORTMSK_E));
	DDRF = ((PORTMSK_F & PORTDIR_F) | (DDRF & ~PORTMSK_F));
	DDRG = ((PORTMSK_G & PORTDIR_G) | (DDRG & ~PORTMSK_G));
	DDRH = ((PORTMSK_H & PORTDIR_H) | (DDRH & ~PORTMSK_H));
	DDRJ = ((PORTMSK_J & PORTDIR_J) | (DDRJ & ~PORTMSK_J));
	DDRK = ((PORTMSK_K & PORTDIR_K) | (DDRK & ~PORTMSK_K));
	DDRL = ((PORTMSK_L & PORTDIR_L) | (DDRL & ~PORTMSK_L));
	
	// Pin Change
	uint8_t pcmsk = 0;
	shadowB = PINB;
	if(USE_PINCHANGE_PB0) pcmsk |= (1<<PCINT0);
	if(USE_PINCHANGE_PB1) pcmsk |= (1<<PCINT1);
	if(USE_PINCHANGE_PB2) pcmsk |= (1<<PCINT2);
	if(USE_PINCHANGE_PB3) pcmsk |= (1<<PCINT3);
	if(USE_PINCHANGE_PB4) pcmsk |= (1<<PCINT4);
	if(USE_PINCHANGE_PB5) pcmsk |= (1<<PCINT5);
	if(USE_PINCHANGE_PB6) pcmsk |= (1<<PCINT6);
	if(USE_PINCHANGE_PB7) pcmsk |= (1<<PCINT7);
	if(pcmsk)
	{
		PCICR = PCICR|(1<<PCIE0);
		PCMSK0 = maskB = pcmsk;
		pcmsk = 0;
	}
	shadowEJ = (PINJ<<1)|(PINE & 0x01);
	if(USE_PINCHANGE_PE0) pcmsk |= (1<<PCINT8);
	if(USE_PINCHANGE_PJ0) pcmsk |= (1<<PCINT9);
	if(USE_PINCHANGE_PJ1) pcmsk |= (1<<PCINT10);
	if(USE_PINCHANGE_PJ2) pcmsk |= (1<<PCINT11);
	if(USE_PINCHANGE_PJ3) pcmsk |= (1<<PCINT12);
	if(USE_PINCHANGE_PJ4) pcmsk |= (1<<PCINT13);
	if(USE_PINCHANGE_PJ5) pcmsk |= (1<<PCINT14);
	if(USE_PINCHANGE_PJ6) pcmsk |= (1<<PCINT15);
	if(pcmsk)
	{
		PCICR = PCICR|(1<<PCIE1);
		PCMSK1 = maskEJ = pcmsk;
		pcmsk = 0;
	}
	shadowK = PINK;
	if(USE_PINCHANGE_PK0) pcmsk |= (1<<PCINT16);
	if(USE_PINCHANGE_PK1) pcmsk |= (1<<PCINT17);
	if(USE_PINCHANGE_PK2) pcmsk |= (1<<PCINT18);
	if(USE_PINCHANGE_PK3) pcmsk |= (1<<PCINT19);
	if(USE_PINCHANGE_PK4) pcmsk |= (1<<PCINT20);
	if(USE_PINCHANGE_PK5) pcmsk |= (1<<PCINT21);
	if(USE_PINCHANGE_PK6) pcmsk |= (1<<PCINT22);
	if(USE_PINCHANGE_PK7) pcmsk |= (1<<PCINT23);
	if(pcmsk)
	{
		PCICR = PCICR|(1<<PCIE2);
		PCMSK2 = maskK = pcmsk;
		pcmsk = 0;
	}
	 
	return 0;
}

// Pin Change Interrupt Routine für die Pins PB0 bis PB7
// Es gibt nur einen Interrupt-Vektor für acht einzelne Pin-Change. Deshalb wird in der ISR als erstes 
// festgestellt, welche Pins sichh verändert haben. Dazu wird der alte Wert i einem Schattenregister 
// (shadowB) gespeichert. Beim Auftreten wird dann der aktuelle PINB-Registerwert mit diesem 
// Schattenregister EXOR verknüpft, wodurch alle Änderungen als 1 im entsprechenden Bit sichtbar werden. 
// Die UND-Verknüpfung mit maskB blendet alle Ändeungen aus, welche gar nicht aktiv überwacht werden 
// sollen.
// Das achtfache if(USE...) Konstrukt prüft alle Rahmenbedingungen ab und ruft, wenn alles o.k. ist, 
// die entsprechende Call-Back Routine auf.
// HINWEIS: Wenn ein Pin-Change auf einem der möglichen Pins Pxy überwacht werden soll, so muss das 
// USE_PINCHANGE_Pxy-Flag als (1) definiert werden und zusätzlich muss die Funktion pinchange_Pxy 
// geschrieben werden. Der Prototyp 'void pinchange_Pxy(uint8_t edge);' ist in HAL_MobiRob.h 
// vordefiniert.


ISR(PCINT0_vect)
{
	uint8_t pin = PINB;							// Eingangsregister B lesen
	uint8_t change = ((pin ^ shadowB) & maskB);	// Exor mit alten Werten -> alle geänderten Bits = 1
	if(change)									// falls 1 (oder mehrere) Bit geändert haben: 
	{											//   die entsprechende(n) Call-Back Routine(n) aufrufen
	  if(USE_PINCHANGE_PB0&&(change&(1<<0))&&cbr[0].valid&&cbr[0].ptr!=NULL) cbr[0].ptr((pin>>0)&0x01); 
	  // falls der entsprechende Pin benützt wird
	  //	UND der entsprechende Pin eine Änderung hat
	  //		UND die Call-Back Struktur initialisiert ist
	  //			UND der Call-Back Funktionszeiger nicht NULL ist:
	  //					Rufe aus dem cbr-Array die entsprechende Call-Back-Funktion auf und
	  //					übergib ihr den Wert des entsprechenden Pins, rechtsbündig und ausmaskiert
	  if(USE_PINCHANGE_PB1&&(change&(1<<1))&&cbr[1].valid&&cbr[1].ptr!=NULL) cbr[1].ptr((pin>>1)&0x01);
	  if(USE_PINCHANGE_PB2&&(change&(1<<2))&&cbr[2].valid&&cbr[2].ptr!=NULL) cbr[2].ptr((pin>>2)&0x01);
	  if(USE_PINCHANGE_PB3&&(change&(1<<3))&&cbr[3].valid&&cbr[3].ptr!=NULL) cbr[3].ptr((pin>>3)&0x01);
	  if(USE_PINCHANGE_PB4&&(change&(1<<4))&&cbr[4].valid&&cbr[4].ptr!=NULL) cbr[4].ptr((pin>>4)&0x01);
	  if(USE_PINCHANGE_PB5&&(change&(1<<5))&&cbr[5].valid&&cbr[5].ptr!=NULL) cbr[5].ptr((pin>>5)&0x01);
	  if(USE_PINCHANGE_PB6&&(change&(1<<6))&&cbr[6].valid&&cbr[6].ptr!=NULL) cbr[6].ptr((pin>>6)&0x01);
	  if(USE_PINCHANGE_PB7&&(change&(1<<7))&&cbr[7].valid&&cbr[7].ptr!=NULL) cbr[7].ptr((pin>>7)&0x01);
	}
	shadowB = pin;

}

// Pin Change Interrupt Routine für die Pins PE0, PJ0 bis PJ6
// Kommentare siehe ISR(PCINT0_vect)
ISR(PCINT1_vect)
{
	uint8_t pin = ((PINJ<<1)|(PINE & 0x01));	// Pin-Werte aus 2 Registern zusammenschustern: 
												// [PINJ.6, PINJ.5 .. PINJ.0, PINE.0]
	uint8_t change = ((pin ^ shadowEJ) & maskEJ);
	if(change)
	{
	  if(USE_PINCHANGE_PE0&&(change&(1<<0))&&cbr[ 8].valid&&cbr[ 8].ptr!= NULL) cbr[ 8].ptr((pin>>0)&1);
	  if(USE_PINCHANGE_PJ0&&(change&(1<<1))&&cbr[ 9].valid&&cbr[ 9].ptr!= NULL) cbr[ 9].ptr((pin>>1)&1);
	  if(USE_PINCHANGE_PJ1&&(change&(1<<2))&&cbr[10].valid&&cbr[10].ptr!= NULL) cbr[10].ptr((pin>>2)&1);
	  if(USE_PINCHANGE_PJ2&&(change&(1<<3))&&cbr[11].valid&&cbr[11].ptr!= NULL) cbr[11].ptr((pin>>3)&1);
	  if(USE_PINCHANGE_PJ3&&(change&(1<<4))&&cbr[12].valid&&cbr[12].ptr!= NULL) cbr[12].ptr((pin>>4)&1);
	  if(USE_PINCHANGE_PJ4&&(change&(1<<5))&&cbr[13].valid&&cbr[13].ptr!= NULL) cbr[13].ptr((pin>>5)&1);
	  if(USE_PINCHANGE_PJ5&&(change&(1<<6))&&cbr[14].valid&&cbr[14].ptr!= NULL) cbr[14].ptr((pin>>6)&1);
	  if(USE_PINCHANGE_PJ6&&(change&(1<<7))&&cbr[15].valid&&cbr[15].ptr!= NULL) cbr[15].ptr((pin>>7)&1);
	}
	shadowEJ = pin;

}

// Pin Change Interrupt Routine für die Pins PK0 bis PK7
// Kommentare siehe ISR(PCINT0_vect)
ISR(PCINT2_vect)
{
	uint8_t pin = PINK;
	uint8_t change = ((pin ^ shadowK) & maskK);
	if(change)
	{
	  if(USE_PINCHANGE_PK0&&(change&(1<<0))&&cbr[16].valid&&cbr[16].ptr!= NULL) cbr[16].ptr((pin>>0)&1);
	  if(USE_PINCHANGE_PK1&&(change&(1<<1))&&cbr[17].valid&&cbr[17].ptr!= NULL) cbr[17].ptr((pin>>1)&1);
	  if(USE_PINCHANGE_PK2&&(change&(1<<2))&&cbr[18].valid&&cbr[18].ptr!= NULL) cbr[18].ptr((pin>>2)&1);
	  if(USE_PINCHANGE_PK3&&(change&(1<<3))&&cbr[19].valid&&cbr[19].ptr!= NULL) cbr[19].ptr((pin>>3)&1);
	  if(USE_PINCHANGE_PK4&&(change&(1<<4))&&cbr[20].valid&&cbr[20].ptr!= NULL) cbr[20].ptr((pin>>4)&1);
	  if(USE_PINCHANGE_PK5&&(change&(1<<5))&&cbr[21].valid&&cbr[21].ptr!= NULL) cbr[21].ptr((pin>>5)&1);
	  if(USE_PINCHANGE_PK6&&(change&(1<<6))&&cbr[22].valid&&cbr[22].ptr!= NULL) cbr[22].ptr((pin>>6)&1);
	  if(USE_PINCHANGE_PK7&&(change&(1<<7))&&cbr[23].valid&&cbr[23].ptr!= NULL) cbr[23].ptr((pin>>7)&1);
	}
	shadowK = pin;

}