#ifndef LCD_EXT_H
#define LCD_EXT_H
#include <stdlib.h>

//-------------------------------------------------------------------------
// Erweiterung von Wolfgang Tschallener
// Bertolt-Brecht-Berufskolleg Duisburg   05.11.2006
//
// Es wurden Funktionen aus stdlib.h benutzt.
//-------------------------------------------------------------------------

void lcd_putf(float zahl, int sges, int snach);
//Ausgabe einer Fließkommazahl mit sges Gesamtstellen. 
//Hiervon sind snach Nachkommastellen.
//Die Nachkollastellen werden gerundet. (TSC)

void lcd_puti(int zahl, int sges);
//Ausgabe der Integerzahl zahl formatiert  mit sges Stellen (TSC)

void lcd_putui(unsigned int zahl, int sges);
//Ausgabe der Integerzahl zahl formatiert  mit sges Stellen (TSC)

void lcd_putui_0(unsigned int zahl, int sges);
//Ausgabe der Integerzahl zahl formatiert  mit sges Stellen
//Leerstellen werden mit 0 aufgefüllt (TSC)

void lcd_putui_hex(unsigned int zahl, int sges);
//Ausgabe der nur-positiven Integerzahl zahl formatiert  mit sges Stellen
// das Ausgabeformat ist hex (TSC)

void lcd_putui_bin(unsigned int zahl, int sges);
//Ausgabe der Integerzahl zahl formatiert  mit sges Stellen(TSC)
// Das Ausgabeformat ist binär. Leerstellen werden mit 0 aufgefüllt.

void lcd_putb(uint8_t a);
//Ausgabe einer 8-Bit-Zahl binär(TSC)

#endif //LCD_EXT_H

