/** ***************************************************************************
*
* Atmel Corporation
*
* \file             : TWI_Master.h
* Compiler          : GNU Toolchain - 3.4.2.1002 GCC_VERSION:4.7.2
* Revision          : \version: 1.15 
* Date              : \date: 20.11.2013 11:31:20 
* Updated by        : \author: D.M.Binggeli 
*
* Supported devices : All devices with a TWI module can be used.
*                     The example is written for the ATmega2560
*
* \brief			: AVR315 - TWI Master Implementation
*
* \brief			: Header file for TWI_Master.c
*                     Include this file in the application.
*
/// ## ported to:
/// ## Atmel AVR (8 bit) GNU Toolchain - 3.4.2.1002 GCC_VERSION:4.7.2
/// ## 20.11.2013 D.M.Binggeli FHNW
*
/// !! adapted to PCA9555 devices and ATmega168 with 8MHz CPU / 0.4MHz I2C
****************************************************************************/
#include <avr/io.h>


/****************************************************************************
  TWI Status/Control register definitions
****************************************************************************/
#define TWI_BUFFER_SIZE 32   /// Set this to the largest message size that will be sent including 
///< address byte.
///< this holds valid for the PCA9555: slave address¦command byte¦data to port 0¦data to port 1
///< see PCA9555.pdf page 10, Fig.10

#define TWI_TWBR            0x02        /// TWI Bit rate Register setting.
                                        /// See Application note for detailed
                                        /// information on setting this value.
///< see AVR315 p.5 Table 1: 8MHz CPU / 0.4MHz SCL -> TWBR = 2

// Not used defines!
//#define TWI_TWPS          0x00        // This driver presumes prescaler = 00

/** **************************************************************************
  Global definitions
****************************************************************************/

extern volatile union TWI_statusReg TWI_statusReg; ///< Status byte holding flags.

/** **************************************************************************
  Function definitions
****************************************************************************/
#define TWI_Transceiver_Busy() ((uint8_t)( TWCR & (1<<TWIE) )) /// Macro definition to check if the transceiver is in use

uint8_t TWI_WriteCmd(uint8_t addr, uint8_t cmd, uint8_t len, uint8_t *pmsg);
uint8_t TWI_Write(uint8_t addr, uint8_t len, uint8_t *pmsg);
uint8_t TWI_Read(uint8_t addr, uint8_t size);
uint8_t TWI_ReadCombined(uint8_t addr, uint8_t len, uint8_t *pmsg, uint8_t size);
void TWI_Master_Initialise(void);
uint8_t TWI_Get_State_Info(void);
void TWI_Start_Transceiver(void);
uint8_t TWI_Get_Data_From_Transceiver(uint8_t *msg, uint8_t msgSize);

/** **************************************************************************
  Bit and byte definitions
****************************************************************************/
#define TWI_READ_BIT  0       /// Bit position for R/W bit in "address byte".
#define TWI_ADR_BITS  1       /// Bit position for LSB of the slave address bits in the init byte.
#define I2C_READ    (1<<TWI_READ_BIT)



/** **************************************************************************
  TWI State codes
****************************************************************************/
/// General TWI Master status codes
#define TWI_START                  0x08  /// START has been transmitted
#define TWI_REP_START              0x10  /// Repeated START has been transmitted
#define TWI_ARB_LOST               0x38  /// Arbitration lost

/// TWI Master Transmitter status codes
#define TWI_MTX_ADR_ACK            0x18  /// SLA+W has been transmitted and ACK received
#define TWI_MTX_ADR_NACK           0x20  /// SLA+W has been transmitted and NACK received
#define TWI_MTX_DATA_ACK           0x28  /// Data byte has been transmitted and ACK received
#define TWI_MTX_DATA_NACK          0x30  /// Data byte has been transmitted and NACK received

/// TWI Master Receiver status codes
#define TWI_MRX_ADR_ACK            0x40  /// SLA+R has been transmitted and ACK received
#define TWI_MRX_ADR_NACK           0x48  /// SLA+R has been transmitted and NACK received
#define TWI_MRX_DATA_ACK           0x50  /// Data byte has been received and ACK transmitted
#define TWI_MRX_DATA_NACK          0x58  /// Data byte has been received and NACK transmitted

/// TWI Slave Transmitter status codes
#define TWI_STX_ADR_ACK            0xA8  /// Own SLA+R has been received; ACK has been returned
#define TWI_STX_ADR_ACK_M_ARB_LOST 0xB0  /// Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
#define TWI_STX_DATA_ACK           0xB8  /// Data byte in TWDR has been transmitted; ACK has been received
#define TWI_STX_DATA_NACK          0xC0  /// Data byte in TWDR has been transmitted; NOT ACK has been received
#define TWI_STX_DATA_ACK_LAST_BYTE 0xC8  /// Last data byte in TWDR has been transmitted (TWEA = “0”); ACK has been received

/// TWI Slave Receiver status codes
#define TWI_SRX_ADR_ACK            0x60  /// Own SLA+W has been received ACK has been returned
#define TWI_SRX_ADR_ACK_M_ARB_LOST 0x68  /// Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
#define TWI_SRX_GEN_ACK            0x70  /// General call address has been received; ACK has been returned
#define TWI_SRX_GEN_ACK_M_ARB_LOST 0x78  /// Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_ACK       0x80  /// Previously addressed with own SLA+W; data has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_NACK      0x88  /// Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
#define TWI_SRX_GEN_DATA_ACK       0x90  /// Previously addressed with general call; data has been received; ACK has been returned
#define TWI_SRX_GEN_DATA_NACK      0x98  /// Previously addressed with general call; data has been received; NOT ACK has been returned
#define TWI_SRX_STOP_RESTART       0xA0  /// A STOP condition or repeated START condition has been received while still addressed as Slave

/// TWI Miscellaneous status codes
#define TWI_NO_STATE               0xF8  /// No relevant state information available; TWINT = “0”
#define TWI_BUS_ERROR              0x00  /// Bus error due to an illegal START or STOP condition

