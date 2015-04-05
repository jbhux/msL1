#ifndef _PCA9555_
#define _PCA9555_

/**
 * \file PCA9555.h
 *
 * \date 20.11.2013 
 * \author	D. M. Binggeli FHNW
 *
 * \brief Echtzeitfähiger Treiber für die Ansteuerung eines I/O-Port-Extenders PCA9555 über die I2C Schnittstelle
 *
 * \attention adapted to TWI_Master.c as described in AVR315: Using the TWI module as I2C master
 *********************************************************************************/
	
// #define PCA_DEBUG

/**
 * \struct u1688tag
 *
 * \brief Struct für ein 16-Bit Register des PCA9555
 * Implementiert den 8-Bit Zugriff auf ein 16bit Register
 * Des Weitern werden Flags für die Kommunikation bereitgestellt
 */
struct u1688tag
{
    struct
    {
	    unsigned char changed:1;///< Portabbild wurde verändert, muss deshalb an den PCA9555 gesendet werden
	    unsigned char sendt:1; ///< Portabbild wurde gesendet
	    unsigned char unusedBits:6; ///< leere Bits
    };
	union ///< Union erlaubt verschiedene Zugriffe auf den selben Speicherplatz
	{
		uint16_t w;     ///< AVR-GCC is little endian:
		uint8_t b[2];   ///< thus w = A1A0 -> b[1]=A1,b[0]=A0
	};
	
};

/**
 * \struct PCA9555tag
 *
 * \brief Struct für das Port-Abbild des PCA9555 Bausteins. 
 *
 */
struct PCA9555tag
{
	struct u1688tag inp; ///< Eingangs-Register
	struct u1688tag out; ///< Ausgangs-Register
	struct u1688tag pol; ///< Polaritäts-Register. Bestimmt Polarität der Einganssignale, 0: unverändert, 1: Signale werden invertiert
	struct u1688tag dir; ///< Richtungsregister für die E/A-Pins. 1: PIN ist ein Eingang (8ung: anders als beim AVR!!) 
	int8_t status; ///< State Machine in exec_PCA steuert die Übertragung des Port-Abbilds zum Baustein
	int8_t init_flag; ///< 1: PCA9555 muss initialisiert werden, d.h. auch pol und dir werden neu übertragen
};

extern struct PCA9555tag pca;

void init_pca(void);
uint8_t get_jswitch(void);
uint8_t exec_pca(void);
void set_lcd_CS(uint8_t val);
void set_lcd_BL(uint8_t val);

//********************************************************************
// Write Port Extension Registers
// 
// reg0: Register port
// reg1: Register port + 1
// port: Command code
//       0 Input port 0
//       1 Input port 1
//       2 Output port 0
//       3 Output port 1
//       4 Polarity Inversion port 0
//       5 Polarity Inversion port 1
//       6 Configuration port 0
//       7 Configuration port 1

void PortExtWrite(uint8_t out1, uint8_t out0, uint8_t port);

//********************************************************************
// Read Port Extension Registers
// 
// reg0: Register port
// reg1: Register port + 1
// port: Command code
//       0 Input port 0
//       1 Input port 1
//       2 Output port 0
//       3 Output port 1
//       4 Polarity Inversion port 0
//       5 Polarity Inversion port 1
//       6 Configuration port 0
//       7 Configuration port 1
void PortExtRead(uint8_t *inp1, uint8_t *inp0, uint8_t port);


//********************************************************************
// Write DDR Port Extension
// Set Data Direction
// reg0: Configuration Register 6
// reg1: Configuration Register 7
// VALUES:
// 0b0: Input
// 0b1: Output (This is comparable to the DDR registers of the AVR)
#define WriteDDRExtPort(reg1, reg0) PortExtWrite(~reg1, ~reg0, 6)

//********************************************************************
// Write POL Port Extension
// Set Input Polarity
// reg0: Polarity Register 6
// reg1: Polarity Register 7
// VALUES:
// 0b0: Direct
// 0b1: Inverse (no comparable register in the AVR existent)
#define WritePOLExtPort(reg1, reg0) PortExtWrite(reg1, reg0, 4)

//********************************************************************
// Write Output Port Extension
// Write Output Registers
// reg0: Output Register 6
// reg1: Output Register 7
// VALUES: binary logic levels
#define WriteExtPort(reg1, reg0) PortExtWrite(reg1, reg0, 2)


//********************************************************************
// Read Input Port Extension
// 
// reg0: Input Register 6
// reg1: Input Register 7
// VALUES: binary logic levels
#define ReadExtPort(reg1, reg0) PortExtRead(reg1, reg0, 0)

//********************************************************************
// Read DDR Port Extension
// 
// reg0: Configuration Register 6
// reg1: Configuration Register 7
// VALUES:
// 0b0: Input
// 0b1: Output (This is comparable to the DDR registers of the AVR)
void ReadDDRExtPort(uint8_t *reg1, uint8_t *reg0);

//********************************************************************
// Read Polarity Port Extension
// 
// Read Input Polarity
// reg0: Polarity Register 6
// reg1: Polarity Register 7
// VALUES:
// 0b0: Direct
// 0b1: Inverse (no comparable register in the AVR existent)
#define ReadPOLExtPort(reg1, reg0) PortExtRead(reg1, reg0, 4)

//********************************************************************
// Read Output Register Port Extension
// 
// Read Output Registers
// reg0: Output Register 6
// reg1: Output Register 7
// VALUES: binary logic levels
#define ReadOutExtPort(reg1, reg0) PortExtRead(reg1, reg0, 2)

#endif
