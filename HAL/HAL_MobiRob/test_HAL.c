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
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "lcd.h"
#include "lcd_ext.h"


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
    unsigned char temp1, temp2;
    unsigned int i;

    DDRL = 0;                         // PL = input 
    PORTL = (0xFF);                   // Pins PL0..PL5 pull-up enabled
    
    do {
        temp1 = PINL;                  // read input
        for(i=0;i<65535;i++);
        temp2 = PINL;                  // read input
        temp1 = (temp1 & temp2);       // debounce input
    } while ( temp1 == 0xFF);

    do {
        temp1 = PINL;                  // read input
        for(i=0;i<65535;i++);
        temp2 = PINL;                  // read input
        temp1 = (temp1 & temp2);       // debounce input
    } while ( temp1 != 0xFF);

}


int main(void)
{
    char buffer[7];
    int  num = 4321;
    float fnum = 123.45678;
    unsigned char i;
    
asm volatile ("ldi r16,0xf0" "\n\t"
    "mov r0,r16" "\n\t"
    "ldi r16,0x15" "\n\t"
    "mov r9,r16" "\n\t"
    "add r0, r9""\n\t"); 


    /* initialize display, cursor off */
    
    lcd_init_samsung(LCD_DISP_ON);
  

    for (;;) {                           /* loop forever */
        /* 
         * Test 1:  write text to display
         */

        /* clear display and home cursor */
        lcd_clrscr();
        
        /* put string to display (line 1) with linefeed */
        lcd_puts("LCD Test Line 1\n");
        /* put string to display (line 1) with linefeed */
        lcd_puts("LCD Test Line 2\n");
        /* put string to display (line 1) with linefeed */
        lcd_puts("LCD Test Line 3\n");


        /* cursor is now on second line, write second line */
        lcd_puts("LCD Test Line 4");
        
        /* move cursor to position 16 on line 2 */
        lcd_gotoxy(15,1);  
        
        /* write single char to display */
        lcd_putc(':');
        
        /* wait until push button is pressed */
        wait_until_key_pressed();
        
        
        /*
         * Test 2: use lcd_command() to turn on cursor
         */
        
        /* move cursor to position 16 on line 2 */
        lcd_gotoxy(0,0);  
        lcd_puts("Test 2: Cursor On\n");
        /* turn on cursor */
        lcd_command(LCD_DISP_ON_CURSOR_BLINK);
        
        wait_until_key_pressed();



        /*
         * Test 3: progmem strings, >20char
         */
        
        lcd_clrscr();     /* clear display home cursor */
        lcd_puts("Test 3: Progmem str\n");
        /* put string from program memory to display */
        lcd_puts_P( "Line 1 longer than 20 characters\n" );
        lcd_puts_P( "Line 2 20 characters" );

        wait_until_key_pressed();


        /* turn off cursor */
        lcd_command(LCD_DISP_ON);
        
                /*
         *   Test 4: Display integer values
         */
        
        lcd_clrscr();   /* clear display home cursor */
        
        /* convert interger into string */
        itoa( num , buffer, 10);
        
        /* put converted string to display */
        lcd_puts(buffer);
        
        wait_until_key_pressed();
        
        
        /*
         *  Test 5: Display userdefined characters
         */

       lcd_clrscr();   /* clear display home cursor */
       
       lcd_puts("Test 5, Copyright:");
       
       /*
        * load two userdefined characters from program memory
        * into LCD controller CG RAM location 0 and 1
        */
       lcd_command(_BV(LCD_CGRAM));  /* set CG RAM start address 0 */
       for(i=0; i<16; i++)
       {
           lcd_data(pgm_read_byte_near(&copyRightChar[i]));
       }
       
       /* move cursor to position 0 on line 2 */
       /* Note: this switched back to DD RAM adresses */
       lcd_gotoxy(0,1);
       
       /* display user defined (c), built using two user defined chars */
       lcd_putc(0);
       lcd_putc(1);
       
       wait_until_key_pressed();
              
        lcd_clrscr();     /* clear display home cursor */
        lcd_puts("Test 6: Float\n");
        lcd_putf(fnum, 8, 3);
        //Ausgabe einer Fließkommazahl mit sges Gesamtstellen. 
        //Hiervon sind snach Nachkommastellen.
        //Die Nachkollastellen werden gerundet. (TSC)
        wait_until_key_pressed();

        lcd_clrscr();     /* clear display home cursor */
        lcd_puts("Test 7: Integer\n");
        lcd_puti(num, 8);
        //Ausgabe der Integerzahl zahl formatiert  mit sges Stellen (TSC)
        wait_until_key_pressed();

        lcd_clrscr();     /* clear display home cursor */
        lcd_puts("Test 8: uint\n");
        lcd_putui(num, 8);
        //Ausgabe der Integerzahl zahl formatiert  mit sges Stellen (TSC)
        wait_until_key_pressed();

        lcd_clrscr();     /* clear display home cursor */
        lcd_puts("Test 9: 00uint\n");
        lcd_putui_0(num, 8);
        //Ausgabe der Integerzahl zahl formatiert  mit sges Stellen
        //Leerstellen werden mit 0 aufgefüllt (TSC)
        wait_until_key_pressed();

        lcd_clrscr();     /* clear display home cursor */
        lcd_puts("Test A: hex\n");
        lcd_putui_hex(num, 12);
        //Ausgabe der nur-positiven Integerzahl zahl formatiert  mit sges Stellen
        // das Ausgabeformat ist hex (TSC)
        wait_until_key_pressed();

        lcd_clrscr();     /* clear display home cursor */
        lcd_puts("Test B: bin\n");
        lcd_putui_bin(num, 20);
        //Ausgabe der Integerzahl zahl formatiert  mit sges Stellen(TSC)
        // Das Ausgabeformat ist binär. Leerstellen werden mit 0 aufgefüllt.
        wait_until_key_pressed();

        lcd_clrscr();     /* clear display home cursor */
        lcd_puts("Test C: 8bit-bin\n");
        lcd_putb(0xA5);
        //Ausgabe einer 8-Bit-Zahl binär(TSC)
        wait_until_key_pressed();

    }
}
