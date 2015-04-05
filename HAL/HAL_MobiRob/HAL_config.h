/*
 * HAL_config.h
 *
 * Created: 24.03.2015 12:50:17
 *  Author: daniel.binggeli
 */ 


#ifndef HAL_CONFIG_H_
#define HAL_CONFIG_H_

#ifndef F_CPU
	#define F_CPU (16000000UL)
#endif // F_CPU

// HINWEIS:
// ------------------------------------------
// Hier können die einzelnen Funktionen des
// HAL konfiguriert werden. 
// Alle Kontroller-Ressourcen, welche mit 
// einem USE_xy-Flag verbunden sind, welches auf (1)
// gesetzt ist, werden automatisch vom HAL belegt und
// stehen nicht mehr zur Verfügung

// WARNUNG:
// ------------------------------------------
// gültige Werte für die USE_xy-Flag sind nur (0)
// und (1). Alle anderen Werte führen zu Fehlern.

// DC_Motoren (1..4)
// #: PWM:     M+:  M-
// 1: OC0B/PG5 PC0  PC1
// 2: OC0A/PB7 PC2  PC3
// 3: OC2A/PB4 PC4  PC5
// 4: OC2B/PH6 PC6  PC7

#define USE_MOTOR_1 (1) // 1: used; 0: not used
#define USE_MOTOR_2 (1)
#define USE_MOTOR_3 (1)
#define USE_MOTOR_4 (1)

// Servos(0..9)
// 0..7:PA0..PA7
// 8, 9:PG2, PJ6
#define USE_SERVO_0 (0) // 1: used; 0: not used
#define USE_SERVO_1 (0) 
#define USE_SERVO_2 (0)
#define USE_SERVO_3 (0)
#define USE_SERVO_4 (0)
#define USE_SERVO_5 (0)
#define USE_SERVO_6 (0)
#define USE_SERVO_7 (0)
#define USE_SERVO_8 (0) 
#define USE_SERVO_9 (0)


// Ultraschall(1..2)
// #  TRIG     ECHO
// 1: OC3B/PE4 INT7/PE7
// 2: OC3A/PE3 INT6/PE6
#define USE_SONIC_1 (0)
#define USE_SONIC_2 (0)

// IR-Sensoren(1..4) ls digitale Eingänge ODER
#define USE_IR_1 (1)
#define USE_IR_2 (1)
#define USE_IR_3 (1)
#define USE_IR_4 (1)

// IR-Sensoren(1..4) als ADC0..ADC3
// Diese Definitionen als (1) hängen die digitale Logik vom Pin ab
#define ANALOG_IR_1 (0)
#define ANALOG_IR_2 (0)
#define ANALOG_IR_3 (0)
#define ANALOG_IR_4 (0)

// HINWEIS: ADC4..ADC7 sind durch JTAG belegt und nicht verfügbar

// Batt, MagnetSensor und Liniensensoren(1..4) als digitale Eingänge ODER
#define USE_BATT   (0)
#define USE_MAG_1  (0)
#define USE_LINE_1 (1)
#define USE_LINE_2 (0)
#define USE_LINE_3 (0)
#define USE_LINE_4 (0)

// Batt, MagnetSensor und Liniensensoren(1..4) als ADC8..ADC13 dann ADC14 und ADC15
// Diese Definitionen als (1) hängen die digitale Logik vom Pin ab 
#define ANALOG_BATT   (1)
#define ANALOG_MAG_1  (0)
#define ANALOG_LINE_1 (0)
#define ANALOG_LINE_2 (0)
#define ANALOG_LINE_3 (0)
#define ANALOG_LINE_4 (0)
#define ANALOG_ADC_14 (0)
#define ANALOG_ADC_15 (0)
				   
// Encoder
#define USE_ENC_1  (1)
#define USE_ENC_2  (1)
#define USE_QUADDEC_1 (1)
#define USE_QUADDEC_2 (1)


// serial communications
#define USE_SPI   (0)
#define USE_UART0 (0)
#define USE_UART2 (0)

// io's
// Hier kann die Funktion der Pin-Change Interrupts aktiviert werden.
// HINWEIS: Wenn ein Pin-Change auf einem der möglichen Pins Pxy überwacht werden soll, so muss das
// USE_PINCHANGE_Pxy-Flag als (1) definiert werden und zusätzlich muss die Funktion pinchange_Pxy
// geschrieben werden. Der Prototyp 'void pinchange_Pxy(uint8_t edge);' ist in HAL_MobiRob.h
// vordefiniert. 			 
#define USE_PINCHANGE_PB0 (0)
#define USE_PINCHANGE_PB1 (0)
#define USE_PINCHANGE_PB2 (0)
#define USE_PINCHANGE_PB3 (0)
#define USE_PINCHANGE_PB4 (0)
#define USE_PINCHANGE_PB5 (0)
#define USE_PINCHANGE_PB6 (0)
#define USE_PINCHANGE_PB7 (0)
#define USE_PINCHANGE_PE0 (0)
#define USE_PINCHANGE_PJ0 (0)
#define USE_PINCHANGE_PJ1 (0)
#define USE_PINCHANGE_PJ2 (0)
#define USE_PINCHANGE_PJ3 (0)
#define USE_PINCHANGE_PJ4 (0)
#define USE_PINCHANGE_PJ5 (0)
#define USE_PINCHANGE_PJ6 (0)
#define USE_PINCHANGE_PK0 (0)
#define USE_PINCHANGE_PK1 (0)
#define USE_PINCHANGE_PK2 (0)
#define USE_PINCHANGE_PK3 (0)
#define USE_PINCHANGE_PK4 (0)
#define USE_PINCHANGE_PK5 (0)
#define USE_PINCHANGE_PK6 (0)
#define USE_PINCHANGE_PK7 (0)

// system
// Hier kann die Funktion des System-Ticks aktiviert und konfiguriert werden.
// HINWEIS: Wenn der System-Tick interrupt gesteuert überwacht werden soll, so muss das
// USE_SYSTEM_TICK-Flag als (1) definiert werden und zusätzlich muss die Funktion system_tick()
// geschrieben werden. Der Prototyp 'void system_tick(uint32_t time);' ist in HAL_MobiRob.h
// vordefiniert.
// Die Konstante SYSTEM_INTERVAL definiert das System-Intervall im [ms].
// HINWEIS: Sinvoll sind Vielfache von 4 im Bereich 4..1024ms.
// Nur für Vielfache von 4 kann ein präzises Einhalten garantiert werden.
// HINWEIS: Die interne Systemzeit wird periodisch jedes SYSTEM_INTERVAL aktualisiert.
// D.h. time_get() liefert immer nur Vielfache von SYSTEM_INTERVAL, jedoch in [ms].
// Das erlaubt es, direkt im [ms] zu rechnen. 

// 
#define USE_SYSTEM_TICK (1)
#define SYSTEM_INTERVAL (4)

#endif /* HAL_CONFIG_H_ */