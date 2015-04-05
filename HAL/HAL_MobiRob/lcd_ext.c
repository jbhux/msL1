//-------------------------------------------------------------------------
// Erweiterung von Wolfgang Tschallener
// Bertolt-Brecht-Berufskolleg Duisburg   05.11.2006
//
// Es wurden Funktionen aus stdlib.h benutzt.
//-------------------------------------------------------------------------
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "lcd.h"
#include "lcd_ext.h"

#define BUFSIZE 20

char buffer[BUFSIZE];

void lcd_putf(float zahl, int sges, int snach){
//Ausgabe einer Fließkommazahl mit sges Gesamtstellen. 
//Hiervon sind snach Nachkommastellen.
//Die Nachkollastellen werden gerundet. (TSC)
if(sges>(BUFSIZE-1)) sges=(BUFSIZE-1);
dtostrf(zahl,sges,snach,buffer);
buffer[(BUFSIZE-1)]='\0';
lcd_puts(buffer);
}


void lcd_puti(int zahl, int sges){
//Ausgabe der Integerzahl zahl formatiert  mit sges Stellen (TSC)
if(sges>(BUFSIZE-1)) sges=(BUFSIZE-1);
uint8_t l=0,n;
char *z=buffer;
itoa(zahl,buffer,10);
while(*z!=0){l++; z++;}//Bufferlänge l
for(n=l;n<sges;n++) lcd_putc(' ');
buffer[(BUFSIZE-1)]='\0';
lcd_puts(buffer);
}

void lcd_putui(unsigned int zahl, int sges){
//Ausgabe der Integerzahl zahl formatiert  mit sges Stellen (TSC)
if(sges>(BUFSIZE-1)) sges=(BUFSIZE-1);
uint8_t l=0,n;
char *z=buffer;
utoa(zahl,buffer,10);
while(*z!=0){l++; z++;}//Bufferlänge l
for(n=l;n<sges;n++) lcd_putc(' ');
buffer[(BUFSIZE-1)]='\0';
lcd_puts(buffer);
}
void lcd_putui_0(unsigned int zahl, int sges){
//Ausgabe der Integerzahl zahl formatiert  mit sges Stellen
//Leerstellen werden mit 0 aufgefüllt (TSC)
if(sges>(BUFSIZE-1)) sges=(BUFSIZE-1);
uint8_t l=0,n;
char *z=buffer;

utoa(zahl,buffer,10);
while(*z!=0){l++; z++;}//Bufferlänge l
for(n=l;n<sges;n++) lcd_putc('0');
buffer[(BUFSIZE-1)]='\0';
lcd_puts(buffer);
}
void lcd_putui_hex(unsigned int zahl, int sges){
//Ausgabe der nur-positiven Integerzahl zahl formatiert  mit sges Stellen
// das Ausgabeformat ist hex (TSC)
if(sges>(BUFSIZE-1)) sges=(BUFSIZE-1);
uint8_t l=0,n;
char *z=buffer;
utoa(zahl,buffer,16);
while(*z!=0){l++; z++;}//Bufferlänge l
for(n=l;n<sges;n++) lcd_putc(' ');
buffer[(BUFSIZE-1)]='\0';
lcd_puts(buffer);
}
void lcd_putui_bin(unsigned int zahl, int sges){
//Ausgabe der Integerzahl zahl formatiert  mit sges Stellen(TSC)
// Das Ausgabeformat ist binär. Leerstellen werden mit 0 aufgefüllt.
if(sges>(BUFSIZE-1)) sges=(BUFSIZE-1);
uint8_t l=0,n;
char *z=buffer;
utoa(zahl,buffer,2);
while(*z!=0){l++; z++;}//Bufferlänge l
for(n=l;n<sges;n++) lcd_putc('0');
buffer[(BUFSIZE-1)]='\0';
lcd_puts(buffer);
}
//Ausgabe einer 8-Bit-Zahl binär(TSC)
void lcd_putb(uint8_t a){
lcd_putui_bin(a,8);
}


