/* This file has been prepared for Doxygen automatic documentation generation.*/
/**  ***************************************************************************
 * \file	PCA9555.c
 * \date	20.11.2013 
 * \author	D. M. Binggeli FHNW
 *
 * \brief Echtzeitfähiger Treiber für die Ansteuerung eines I/O-Port-Extenders PCA9555 über die I2C Schnittstelle
 *
 * \attention adapted to TWI_Master.c as described in AVR315: Using the TWI module as I2C master
 *********************************************************************************/

#include <avr/io.h>
#include "HALlocal.h"
#include "TWI_Master.h"

#include "PCA9555.h"


#define PCA9555 0x40      	/// device address of port expander PCA9555
#define I2C_READ    1
#define I2C_WRITE   0

/** \struct pca
 \brief Konfiguration der Ein- und Ausgänge des PCA9555 IO Port Extender
 
 \n I/O0.0: (input) Jkey down (B) 
 \n I/O0.1: (input) Jkey right (A)
 \n I/O0.2: (input) Jkey center (CEN)
 \n I/O0.3: (input) Jkey up (C)
 \n I/O0.4: (input) Jkey left (D)
 \n I/O0.5: (output) LCD Backlight
 \n I/O0.6: (output) LCD ¬CS
 \n I/O0.7: (i/o)PAD3
 \n I/O1.0: (i/o)PAD4
 \n I/O1.1: (i/o)PAD5
 \n I/O1.2: (i/o)PAD6
 \n I/O1.3: (i/o)PAD7
 \n I/O1.4: (i/o)PAD8
 \n I/O1.5: (i/o)PAD9
 \n I/O1.6: (i/o)PAD10
 \n I/O1.7: (i/o)PAD11
**/

///\brief Richtungsdefinition für den PCA9555 IO Port Extender
/// 1 = input
/// 0 = output
#define PCA_DIR (0b1111111110011111)
///\brief Eingangs-Polaritätsdefinition für den PCA9555 IO Port Extender
#define PCA_POL (0b0000000000000000)
///\brief Liest die untere Hälfte eines 16-Bit PCA-Regsiters
#define LOW(x)	((uint8_t)(x))
///\brief Liest die obere Hälfte eines 16-Bit PCA-Regsiters
#define HIGH(x) ((uint8_t)((x)>>8))


/**
 * \struct pca
 *
 * \brief Struct für das Port-Abbild des PCA9555 Bausteins. 
 *
 */
struct PCA9555tag pca;

//! \enum PCA_STATUS
//! \brief Definiert die Schritte der PCA-State-Machine
enum PCA_STATUS
{
	PCA_INIT = 0, /// PCA Initialisieren
	PCA_WDIR,	/// Datenrichtungsregister schreiben
	PCA_WPOL,   /// Polaritätsregister schreiben
	PCA_WOUT,	/// Ausgangsregister schreiben
	PCA_RIN1,   /// Eingansregister adressieren
	PCA_RIN2,	/// Eingansregister Wert lesen
	PCA_RIN3,	/// - nicht benützt -
	PCA_INVALID, /// Schlussmarkierung, nur zum Fehler abfangen
};

//! \enum PCA_REGISTERS
//! \brief Nummeriert die 8 8-Bit Register des PCA9555, wie sie im Baustein vorliegen
enum PCA_REGISTERS
{
	PCA_I0 = 0,
	PCA_I1,
	PCA_O0,
	PCA_O1,
	PCA_N0,
	PCA_N1,
	PCA_C0,
	PCA_C1,
};

/** *************************************************************************
 * \brief initialisiert das Portabbild pca des PCA9555 und startet die Initialisierung des Bausteins
*****************************************************************************/ 
void init_pca(void)
{
	pca.dir.b[1] = HIGH(PCA_DIR);
	pca.dir.b[0] = LOW(PCA_DIR);
	pca.dir.changed = TRUE;
	pca.pol.b[1] = HIGH(PCA_POL);
	pca.pol.b[0] = LOW(PCA_POL);
	pca.pol.changed = TRUE;
	pca.out.b[1] = 0b00000000;
	pca.out.b[0] = 0b01000000;
	pca.out.changed = TRUE;
	pca.inp.b[1] = 0;
	pca.inp.b[0] = 0;
	pca.status = PCA_INIT;
	pca.init_flag = 1;
}

struct
{
	uint8_t in[4];
	uint8_t out[4];
	uint8_t numi;
	uint8_t numo;
} twibuf;

/** *************************************************************************
 * \brief Zustandsmaschine für die Ansteuerung des Bausteins PCA9555
 * \return slot_used TRUE: Slotzeit verbraucht, FALSE: keine Slotzeit verbraucht
 *
 * \attention sollte  1x pro 4ms aufgerufen werden, damit der 
 * Port extender am I2C-Bus  * möglichst effizient bedient wird
 *
*****************************************************************************/  

 uint8_t exec_pca(void)
{
	uint8_t slot_used = FALSE;
	
#ifdef PCA_DEBUG	
	PORTB |= _BV(PB0);  // Grüne LED zum Test
#endif
	
// 	if(adc_busy) slot_used = FALSE;
// 	else
	{
		switch(pca.status)
		{
			case PCA_INIT:
			init_pca();
			TWI_Master_Initialise();
			pca.status = PCA_WDIR;
			slot_used = TRUE;
			pca.init_flag = 0;
			break;
			case PCA_WDIR:
			/// check if data has changed
			if(pca.dir.changed)
			{
				/// when ready, transmit direction register write command
				if(!TWI_Transceiver_Busy())
				{
					// compose message for direction register write command
					twibuf.numo = 4;
					twibuf.numi = 0;
					twibuf.out[0] = PCA9555+I2C_WRITE;
					twibuf.out[1] = PCA_C0;
					twibuf.out[2] = pca.dir.b[0];
					twibuf.out[3] = pca.dir.b[1];
					TWI_Start_Transceiver_With_Data(twibuf.out, twibuf.numo);
					pca.status	= PCA_WPOL;
					pca.dir.sendt = TRUE;
					slot_used = TRUE;
				}
			}
			break;
			case PCA_WPOL:
			/// when ready, transmit polarity register write command
			if(!TWI_Transceiver_Busy())
			{
				// if WDIR cycle has occurred: reset flags
				if(pca.dir.sendt) pca.dir.changed = pca.dir.sendt = FALSE;
				// send only if data has changed
				if(pca.pol.changed)
				{
					// compose message for polarity inversion  register write command
					twibuf.numo = 4;
					twibuf.numi = 0;
					twibuf.out[0] = PCA9555+I2C_WRITE;
					twibuf.out[1] = PCA_N0;
					twibuf.out[2] = pca.pol.b[0];
					twibuf.out[3] = pca.pol.b[1];
					TWI_Start_Transceiver_With_Data(twibuf.out, twibuf.numo);
					pca.pol.sendt = TRUE;
					slot_used = TRUE;
				}
				pca.status	= PCA_WOUT;
			}
			break;
			case PCA_WOUT:
			/// when ready, transmit output register write command
			if(!TWI_Transceiver_Busy())
			{
				// if WPOL cycle has occurred: reset flags
				if(pca.pol.sendt) pca.pol.changed = pca.pol.sendt = FALSE;
				// send only if data has changed
				if(pca.out.changed)
				{
					// compose message for output register write command
					twibuf.numo = 4;
					twibuf.numi = 0;
					twibuf.out[0] = PCA9555+I2C_WRITE;
					twibuf.out[1] = PCA_O0;
					twibuf.out[2] = pca.out.b[0];
					twibuf.out[3] = pca.out.b[1];
					TWI_Start_Transceiver_With_Data(twibuf.out, twibuf.numo);
					pca.out.sendt = TRUE;
					slot_used = TRUE;
				}
				pca.status	= PCA_RIN1;
			}
			break;
			case PCA_RIN1:
			/// when ready, transmit input register read command
			if(!TWI_Transceiver_Busy())
			{
				// if WOUT cycle has occurred: reset flags
				if(pca.out.sendt) pca.out.changed = pca.out.sendt = FALSE;
				// compose message to command the input register read procedure
				twibuf.numo = 2;  // two bytes to be sent
				twibuf.numi = 2;  // this is the number of requested bytes from the slave
				twibuf.out[0] = PCA9555+I2C_WRITE;
				twibuf.out[1] = PCA_I0;
				TWI_Start_Transceiver_With_Data(twibuf.out, twibuf.numo);
				pca.status	= PCA_RIN2;
				slot_used = TRUE;
			}
			break;
			case PCA_RIN2:
			/// when ready, start reception of input register data
			if(!TWI_Transceiver_Busy())
			{
				// read the received bytes
				TWI_Get_Data_From_Transceiver(twibuf.in, twibuf.numi);
				pca.inp.b[0]=twibuf.in[0];
				pca.inp.b[1]=twibuf.in[1];	
				if(pca.init_flag) pca.status = PCA_INIT; // re-initialise
				else pca.status	= PCA_WOUT; /// loop to PCA_WOUT
				slot_used = TRUE;
			}
			break;
			case PCA_INVALID:
			default:
			pca.status = PCA_INIT;
			break;
		}
	}
#ifdef PCA_DEBUG
	PORTB &= ~(_BV(PB0));  // Grüne LED zum Test
#endif	
		
	return slot_used;
}

// uint8_t get_jswitch(void)
// {
// 	uint8_t val;
// 	val = ~pca.inp.b[0];
// 
// 	return (val & 0x1F);
// }
/** *****************************************************************
 * \brief Aktiviere den Chip Select des LCD-Moduls
 ********************************************************************/
void set_lcd_CS(uint8_t val)
{
	if(val&1) pca.out.b[0] = ((val & 1)<<6);
	else pca.out.b[0] = ((val & 1)<<6);
	pca.out.changed = TRUE;
}
/** *****************************************************************
 * \brief Aktiviere die Hintergrundbeleuchtung des LCD-Moduls
 * \param val=TRUE: einschalten, FALSE= ausschalten
 ********************************************************************/
void set_lcd_BL(uint8_t val)
{
	if(val!=0) pca.out.b[0] = 0;
	else pca.out.b[0] = (1<<5);
	pca.out.changed = TRUE;
}
/** ******************************************************************
// \brief Schreibe die Port Extension Registers
//
// \param reg0: Register port
// \param reg1: Register port + 1
// \param port: Command code
//       0 Input port 0
//       1 Input port 1
//       2 Output port 0
//       3 Output port 1
//       4 Polarity Inversion port 0
//       5 Polarity Inversion port 1
//       6 Configuration port 0
//       7 Configuration port 1
************************************************************************/
void PortExtWrite(uint8_t out1, uint8_t out0, uint8_t port)
{
	switch(port & 0x06)
	{
		case 0:
		pca.inp.b[0] = out0;
		pca.inp.b[1] = out1;
		pca.init_flag = 0;
		break;
		case 2:
		pca.out.b[0] = out0;
		pca.out.b[1] = out1;
		pca.init_flag = 0;
		break;
		case 4:
		pca.pol.b[0] = out0;
		pca.pol.b[1] = out1;
		pca.init_flag = 1;
		break;
		case 6:
		pca.dir.b[0] = out0;
		pca.dir.b[1] = out1;
		pca.init_flag = 1;
		break;
	}
}

/** ******************************************************************
// \brief Lies die Port Extension Registers
//
// \param reg0: Register port
// \param reg1: Register port + 1
// \param port: Command code
//       0 Input port 0
//       1 Input port 1
//       2 Output port 0
//       3 Output port 1
//       4 Polarity Inversion port 0
//       5 Polarity Inversion port 1
//       6 Configuration port 0
//       7 Configuration port 1
************************************************************************/
void PortExtRead(uint8_t *inp1, uint8_t *inp0, uint8_t port)
{
	switch(port & 0x06)
	{
		case 0:
		*inp0 = pca.inp.b[0];
		*inp1 = pca.inp.b[1];
		break;
		case 2:
		*inp0 = pca.out.b[0];
		*inp1 = pca.out.b[1];
		break;
		case 4:
		*inp0 = pca.pol.b[0];
		*inp1 = pca.pol.b[1];
		break;
		case 6:
		*inp0 = pca.dir.b[0];
		*inp1 = pca.dir.b[1];
		break;
	}
}
/********************************************************************
// Read DDR Port Extension
//
// reg0: Configuration Register 6
// reg1: Configuration Register 7
// VALUES:
// 0b0: Input
// 0b1: Output (This is comparable to the DDR registers of the AVR)
void ReadDDRExtPort(uint8_t *reg1, uint8_t *reg0)
{
	*reg0 = ~pca.dir.b[0];
	*reg1 = ~pca.dir.b[1];
}

***/