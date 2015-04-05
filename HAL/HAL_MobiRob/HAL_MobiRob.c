/*
* HAL_MobiRob.c
*
* Created: 04.03.2015 17:24:39
*  Author: Daniel
*/
/*************************************************************************
Title:    testing output to a HD44780 based LCD display.
Author:   Peter Fleury  <pfleury@gmx.ch>  http://jump.to/fleury
File:     $Id: test_lcd.c,v 1.6 2004/12/10 13:53:59 peter Exp $
Software: AVR-GCC 3.3
Hardware: HD44780 compatible LCD text display
ATS90S8515/ATmega if memory-mapped LCD interface is used
any AVR with 7 free I/O pins if 4-bit IO port mode is used
**************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "HAL_MobiRob.h"
#include "SparkFun_APDS9960.h"

// Global variables
int32_t encval;
char s[20] = "";

// Global Variables for SparkFun_APDS9960
struct SparkFun_APDS9960T apds = APDS_INITIALIZER;
uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;

void setup_adps()
{
	lcd_gotoxy(0,0);
	lcd_puts_P("--------------------");
	lcd_puts_P("APDS9960 ColorSensor");
	lcd_puts_P("--------------------");
	
	// Initialize APDS-9960 (configure I2C and initial values)
	if ( apds.init() ) {
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts_P("adps.init complete");
		} else {
		lcd_gotoxy(0,0);
		lcd_puts_P("adps.init went wrong");
	}
	
	// Start running the APDS-9960 light sensor (no interrupts)
	if ( apds.enableLightSensor(false) ) {
		lcd_puts_P("LightSensor is up");
		} else {
		lcd_puts_P("L_Sensor went wrong");
	}
	
	// Wait for initialization and calibration to finish
	_delay_ms(500);
}
void loop_adps() {
	
	lcd_clrscr();
	lcd_gotoxy(0,0);
	// Read the light levels (ambient, red, green, blue)
	if (  !apds.readAmbientLight(&ambient_light) ||
	!apds.readRedLight(&red_light) ||
	!apds.readGreenLight(&green_light) ||
	!apds.readBlueLight(&blue_light) ) {
		lcd_puts_P("adps.read Error ");
		} else {
		lcd_gotoxy(0,0);
		lcd_puts_P("Ambient:");
		utoa(ambient_light,s,10);
		lcd_puts(s);
		lcd_gotoxy(0,1);
		lcd_puts_P(" Red:   ");
		utoa(red_light,s,10);
		lcd_puts(s);
		lcd_gotoxy(0,2);
		lcd_puts_P(" Green: ");
		utoa(green_light,s,10);
		lcd_puts(s);
		lcd_gotoxy(0,3);
		lcd_puts_P(" Blue:  ");
		utoa(blue_light,s,10);
		lcd_puts(s);
	}
	
	// Wait 1 second before next reading
	_delay_ms(1000);
}
/*
** constant definitions
*/
static const PROGMEM unsigned char copyRightChar[] =
{
	0x07, 0x08, 0x13, 0x14, 0x14, 0x13, 0x08, 0x07,
	0x00, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x00
};


/*
** function prototypes
*/
void wait_until_key_pressed(void);

void wait_until_key_pressed(void)
{
	while(!key_check());
	key_get();
}

uint8_t is_key_pressed(void)
{
	return key_get();
}


void init()
{
	/* initialize display, cursor off */
	
	lcd_init(LCD_DISP_ON);
	//	port_init();
	system_timer_init();
	//	servo_init();
	//	motor_init();
	//	enc_init();
	key_init();
	//	adc_init(AVCC);
	
	sei();

	
}

int main(void)
{
	int  num = 4321;
	float fnum = 123.45678;
	unsigned char i;
	int linienwert=0;
	
	init();

	for (;;) {                           /* loop forever */
		/*
		* Test 1:  write text to display
		*/
		/* clear display and home cursor */
		lcd_clrscr();
		/* put string to display (line 1) with linefeed */
		lcd_puts_P("MobiRob Test Line 1\n");
		/* put string to display (line 1) with linefeed */
		lcd_puts_P("MobiRob Test Line 2\n");
		/* put string to display (line 1) with linefeed */
		lcd_puts_P("MobiRob Test Line 3\n");
		/* cursor is now on second line, write second line */
		lcd_puts_P("MobiRob Test Line 4");
		/* move cursor to position 16 on line 2 */
		lcd_gotoxy(15,1);
		/* write single char to display */
		lcd_putc(':');
		/* wait until push button is pressed */
		wait_until_key_pressed();
		// Test 2: test servo 0
		
		
		//Test: Linie
		lcd_gotoxy(0,0);
		lcd_puts_P("Test Linie\n");
		
		linienwert=port_get(PK2);
		lcd_gotoxy(1,0);
		lcd_puts_P("Linienwert: "+linienwert+"\n");
		
		
		
		
		//	motor_set(1, 1000);
		wait_until_key_pressed();
//----------------------------------------- BIRKIS WORK -------------------------------------------------		
		// Magnetfeld lesen:
		
		// zu Magnetfeld fahren
		// fahre XY Schritte vorwärts
		// Poller gefunden
		// Ausrichten (Mikroschalter)
		// Fahrzeug in Position
		// Messen
		// Auf Display ausgeben
		
		
		
		// Linie folgen:
		int linie = 0;
		
		switch (linie) {
			case 0: // Linie Mitte
			motor_set(1,1000); // Motor
			motor_set(1,1000); // Motor
			break;
			case 1: // Linie links
			motor_set(1,200); // Motor
			motor_set(2,1000); // Motor
			case 2: // Linie rechts
			motor_set(1,1000); // Motor
			motor_set(2, 200); // Motor
		}
		
		// Bande folgen:
		int bande = 0; //return Wert der Abfrage
		
		switch (bande) {
			case 0:
			motor_set(1,1000);
			motor_set(1,1000);
			case 1: // Rechts zu nah
			motor_set(1,1000);
			motor_set(2,700);
			case 2: // Links zu nah
			motor_set(2,1000);
			motor_set(1,700);
		}
		
		
		/*	Encoder
		// Test 5: Encoder einlesen
		
		//lcd_clrscr();   clear display home cursor
		lcd_puts_P("Test 5, Encoder:");
		
		i = 1;
		direction_set(2, +1);
		while(!is_key_pressed())
		{
		volatile static uint32_t t;
		encval=enc_get(i);
		{
		lcd_gotoxy(0,i);
		lcd_puts_P("...........");
		ltoa(encval,s,10);
		lcd_gotoxy(0,i);
		lcd_puts(s);
		}
		(i>=2)?(i=1):(i=2);
		t = time_get()+20;
		// .......
		while (t>time_get());
		//_delay_ms(20);
		}
		*/ //

		/*
		// Test 6: ADC einlesen
		
		//lcd_clrscr();   clear display home cursor
		lcd_puts_P("Test 6, ADC");
		
		i = 1;
		direction_set(2, +1);
		while(!is_key_pressed())
		{
		volatile static uint32_t t;
		num=adc_read(ADC9_SINGLE); // MAGNET
		{
		lcd_gotoxy(0,1);
		lcd_puts_P("MAG:     ");
		itoa(num,s,10);
		lcd_gotoxy(4,1);
		lcd_puts(s);
		}
		t = time_get()+20;
		while (t>time_get());
		//_delay_ms(20);
		num=adc_read(ADC10_SINGLE); // LINE_1
		{
		lcd_gotoxy(0,2);
		lcd_puts_P("L-1:     ");
		itoa(num,s,10);
		lcd_gotoxy(4,2);
		lcd_puts(s);
		}
		t = time_get()+20;
		while (t>time_get());
		//_delay_ms(20);
		encval=enc_get(1);
		{
		lcd_gotoxy(9,1);
		lcd_puts_P("M1.........");
		ltoa(encval,s,10);
		lcd_gotoxy(11,1);
		lcd_puts(s);
		}
		t = time_get()+20;
		while (t>time_get());
		encval=enc_get(2);
		{
		lcd_gotoxy(9,2);
		lcd_puts_P("M2.........");
		ltoa(encval,s,10);
		lcd_gotoxy(11,2);
		lcd_puts(s);
		}
		t = time_get()+20;
		while (t>time_get());
		}
		
		*/
		
		{
			
		
		//
		//// Test 7: I2C
//
		//setup_adps();
		//while(!is_key_pressed())
		//{
			//loop_adps();
		//}
		//
		///*
		//*  Test 5: Display userdefined characters
		//*/
//
		//lcd_clrscr();   /* clear display home cursor */
		//
		//lcd_puts_P("Test 7, Copyright:");
		//
		///*
		//* load two userdefined characters from program memory
		//* into LCD controller CG RAM location 0 and 1
		//*/
		//lcd_command(_BV(LCD_CGRAM));  /* set CG RAM start address 0 */
		//for(i=0; i<16; i++)
		//{
			//lcd_data(pgm_read_byte_near(&copyRightChar[i]));
		//}
		//
		///* move cursor to position 0 on line 2 */
		///* Note: this switched back to DD RAM adresses */
		//lcd_gotoxy(0,1);
		//
		///* display user defined (c), built using two user defined chars */
		//lcd_putc(0);
		//lcd_putc(1);
		//
		//wait_until_key_pressed();
		//
		//lcd_clrscr();     /* clear display home cursor */
		//lcd_puts_P("Test 8: Float\n");
		//lcd_putf(fnum, 8, 3);
		////Ausgabe einer Fließkommazahl mit sges Gesamtstellen.
		////Hiervon sind snach Nachkommastellen.
		////Die Nachkollastellen werden gerundet. (TSC)
		//wait_until_key_pressed();
//
		//lcd_clrscr();     /* clear display home cursor */
		//lcd_puts_P("Test 9: bin\n");
		//lcd_putui_bin(num, 20);
		////Ausgabe der Integerzahl zahl formatiert  mit sges Stellen(TSC)
		//// Das Ausgabeformat ist binär. Leerstellen werden mit 0 aufgefüllt.
		//wait_until_key_pressed();
		//
		}
	}
}
