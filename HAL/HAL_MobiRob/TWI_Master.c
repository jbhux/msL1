/** ***************************************************************************
* Atmel Corporation
*
* \file              : TWI_Master.c
* Compiler          : GNU Toolchain - 3.4.2.1002 GCC_VERSION:4.7.2
* Revision          : \version: 1.15 
* Date              : \date: 20.11.2013 11:31:20
* Updated by        : \author: D.M.Binggeli 
*
* 
* Supported devices : All devices with a TWI module can be used.
*                     The example is written for the ATmega2560
*
* \brief           : AVR315 - TWI Master Implementation
*
* \brief       : This is a sample driver for the TWI hardware modules.
* It is interrupt driven. All functionality is controlled through 
* passing information to and from functions. 
*
***************************************************************************/


#include <avr/io.h>              
#include <avr/interrupt.h>
#include <stdbool.h>
#include "TWI_Master.h"

#define TW_ENABLE ((1<<TWEN)|(1<<TWINT))
#define TW_ARM (TW_ENABLE|(1<<TWIE))
#define TW_START (TW_ARM|(1<<TWSTA))
#define TW_STOP (TW_ENABLE|(1<<TWSTO))
#define TW_ACK (TW_ARM|(1<<TWEA))
#define TW_NAK (TW_ARM)

enum TWMODE_T {TWMODE_IDLE, TWMODE_WRITE, TWMODE_READ, TWMODE_COMBINED};

static struct TWI_T
{
	uint8_t buf[TWI_BUFFER_SIZE];
	uint8_t txsize;
	uint8_t rxsize;
	enum TWMODE_T mode;
	uint8_t state;
	uint8_t lastTransOK;
} twi = {.buf={0},.txsize=0,.rxsize=0,.mode=TWMODE_IDLE,.state=TWI_NO_STATE,.lastTransOK=0};

/** **************************************************************************
\brief Call this function to give all the data and start a TWI write operation
 with a 
This routine may be called regardless of any other ongoing TWI activity.
\param addr TWI device address (7 byte format 1..127) of the target slave.
\param cmd TWI command byte.
\param len bytes of the message will be sent.
\param pmsg pointer to message array.
\return -1 - tx buffer too small, no transmission.
		0 - TWI is busy, no transmission.
		1 - Transmission was successfully initiated

****************************************************************************/
uint8_t TWI_WriteCmd(uint8_t addr, uint8_t cmd, uint8_t len, uint8_t *pmsg)
{
  if(TWI_Transceiver_Busy()) return 0;
  else
  { 
	twi.mode = TWMODE_WRITE;
	twi.txsize = len+2;
	twi.rxsize = 0;
	if(twi.rxsize>=TWI_BUFFER_SIZE) return -1;
    else
	{  
	  twi.buf[0] = (addr<<TWI_ADR_BITS);
	  twi.buf[1] = cmd;
	  for(uint8_t i=2; i<twi.txsize; i++)
	  {
		twi.buf[i] = *pmsg++;
	  }
	  TWI_Start_Transceiver();
	  return 1;
	}
  }    
}

/** **************************************************************************
\brief Call this function to give all the data and start a TWI write operation
 with a 
This routine may be called regardless of any other ongoing TWI activity.
\param addr TWI device address (7 byte format 1..127) of the target slave.
\param len bytes of the message will be sent.
\param pmsg pointer to the message array.
\return -1 - tx buffer too small, no transmission.
		0 - TWI is busy, no transmission.
		1 - Transmission was successfully initiated

****************************************************************************/
uint8_t TWI_Write(uint8_t addr, uint8_t len, uint8_t *pmsg)
{
  if(TWI_Transceiver_Busy()) return 0;
  else
  { 
	twi.mode = TWMODE_WRITE;
	twi.txsize = len+1;
	twi.rxsize = 0;
	if(twi.rxsize>=TWI_BUFFER_SIZE) return -1;
    else
	{  
	  twi.buf[0] = (addr<<TWI_ADR_BITS);
	  for(uint8_t i=1; i<twi.txsize; i++)
	  {
		twi.buf[i] = *pmsg++;
	  }
	  TWI_Start_Transceiver();
	  return 1;
	}
  }    
}
/** **************************************************************************
\brief Call this function to give all the data and start a TWI read operation.
This routine may be called regardless of any other ongoing TWI activity.
\param addr TWI device address (7 byte format 1..127) of the target slave.
\param size bytes will be received and stored into the array of values.
\return -1 - rx buffer too small, no read operation.
        0 - TWI is busy, no read started.
		1 - Read operation was successfully initiated
\attention Call the function TWI_Get_Data_From_Transceiver in order to get 
the requested data from the TWI transceiver buffer.
****************************************************************************/
uint8_t TWI_Read(uint8_t addr, uint8_t size)
{
  if(TWI_Transceiver_Busy()) return 0;
  else
  { 
	twi.mode = TWMODE_READ;
	twi.txsize = 1;
	twi.rxsize = size+1;
	if(twi.txsize>=TWI_BUFFER_SIZE) return -1;
    else
	{  
	  twi.buf[0] = ((addr<<TWI_ADR_BITS)|I2C_READ);
	  TWI_Start_Transceiver();
	  return 1;
	}
  }    
}
/** **************************************************************************
\brief Call this function to give all the data and start a combined 
TWI read operation.
This routine may be called regardless of any other ongoing TWI activity.
\param addr TWI device address (7 byte format 1..127) of the target slave.
\param len message bytes will be transmitted first.
\param pmsg pointer to the message array to be sent.
\param size bytes will be received and stored into the array of values.
\return -1 - rx or tx buffer too small, no operation.
		0 - TWI is busy, no transmission.
		1 - Transmission was successfully initiated
\attention It is the responsibility of the caller to make sure that the 
destination array is large enough.
\attention Call the function TWI_Get_Data_From_Transceiver in order to get 
the requested data from the TWI transceiver buffer.
****************************************************************************/
uint8_t TWI_ReadCombined(uint8_t addr, uint8_t len, uint8_t *pmsg, uint8_t size)
{
  if(TWI_Transceiver_Busy()) return 0;
  else
  { 
	twi.mode = TWMODE_COMBINED;
	twi.txsize = len+1;
	twi.rxsize = size;
	if((twi.rxsize>=TWI_BUFFER_SIZE)||(twi.rxsize>=TWI_BUFFER_SIZE)) return -1;
    else
	{  
	  twi.buf[0] = (addr<<TWI_ADR_BITS);
	  for(uint8_t i=1; i<twi.txsize; i++)
	  {
		twi.buf[i] = *pmsg++;
	  }
	  TWI_Start_Transceiver();
	  return 1;
	}
  }    
}

/** **************************************************************************
\brief Call this function to set up the TWI master to its initial standby state.
Remember to enable interrupts from the main application after initializing the TWI.
****************************************************************************/
void TWI_Master_Initialise(void)
{
  TWBR = TWI_TWBR;        // Set bit rate register (Baud rate). Defined in header file.
  TWDR = 0xFF;            // Default content = SDA released.
  TWCR = (1<<TWEN);       // Enable TWI-interface and release TWI pins.
}					      // Disable interrupt. No Signal requests.
						  
/** ************************************************************************
\brief Call this function to fetch the state information of the previous operation. 
\attention The function will hold execution (loop) until the TWI_ISR has completed with the previous 
operation. If there was an error, then the function will return the TWI State code. 
****************************************************************************/
uint8_t TWI_Get_State_Info(void)
{
  while (TWI_Transceiver_Busy());   // Wait until TWI has completed the transmission.
  return twi.state;                 // Return error state.
}

/** **************************************************************************
\brief Call this function to send a prepared message. 
The first byte must contain the slave address and the read/write bit. 
Consecutive bytes contain the data to be sent, or empty locations for data to be read
from the slave. Also include how many bytes that should be sent/read including the address byte.
\attention The function will hold execution (loop) until the TWI_ISR has completed with 
the previous operation, then initialize the next operation and return.
****************************************************************************/
void TWI_Start_Transceiver(void)
{
  while ( TWI_Transceiver_Busy() );  // Wait until TWI is ready for next transmission.

  twi.lastTransOK	= 0;      
  twi.state         = TWI_NO_STATE ;
  TWCR				= TW_START;		// Initiate a START condition.
}

/** **************************************************************************
\brief Call this function to read out the requested data from the TWI transceiver buffer. 
I.e. first call a Read function to send a request for data to the slave. Then Run this function 
to collect the data when they have arrived. 
\param msg pointer to where to place the data.
\param msgSize the number of bytes requested in the function call.
\return the number of actually read and returned bytes. 
\attention The function will hold execution (loop) until the TWI_ISR has completed with 
the previous operation before reading out the data and returning. 
\attention It is the responsibility of the caller to make sure that the 
destination array is large enough.
****************************************************************************/
uint8_t TWI_Get_Data_From_Transceiver(uint8_t *msg, uint8_t msgSize)
{
  uint8_t i=0;

  if((msgSize>TWI_BUFFER_SIZE-1)||(msgSize==0)) i=0;
  else 
  {
	  while (TWI_Transceiver_Busy());   // Wait until TWI is ready.
	  if(twi.rxsize<msgSize) msgSize=twi.rxsize; // do not copy invalid buffer data
	  if(twi.lastTransOK)				// Last transmission completed successfully.              
	  {                                     
		for (i=0;i<msgSize;i++)         // Copy data from transceiver buffer.
		{
		  msg[i]=twi.buf[1+i];
		}
	  }
  }
  return i;                             // return the number of read bytes
}

// ********** Interrupt Handler ********** //
/****************************************************************************
\brief This function is the Interrupt Service Routine (ISR), and called when the TWI interrupt is triggered;
that is whenever a TWI event has occurred. 
\attention This function should not be called directly from the main application.
****************************************************************************/

ISR (TWI_vect)
{
	static uint8_t twi_idx;

	switch (TWSR)
	{
	case TWI_START:             // START has been transmitted  
	case TWI_REP_START:         // Repeated START has been transmitted
		twi_idx = 0;			// Set buffer pointer to the TWI Address location
	case TWI_MTX_ADR_ACK:       // SLA+W has been transmitted and ACK received
	case TWI_MTX_DATA_ACK:      // Data byte has been transmitted and ACK received
		if (twi_idx < twi.txsize)
		{
			TWDR = twi.buf[twi_idx++];
			TWCR = TW_ARM;		// Enable TWI interrupt and clear the flag
		}
		else					// Send STOP or RESTART after last byte
		{
			twi.lastTransOK = true;	// Set status bits to completed successfully.
			switch(twi.mode)
			{
			default:
			case TWMODE_WRITE:	// it was a register write-command
				TWCR = TW_STOP;	// initiate a STOP condition.
				break;
			case TWMODE_COMBINED:// it was a combined write/read-command
				twi.buf[0] |= I2C_READ; // set 'read'-bit of the TWI Address
				TWCR = TW_START;// initiate a (RE-)START condition.
			}
		}
		break;
	case TWI_MRX_DATA_ACK:      // Data byte has been received and ACK transmitted
		twi.buf[twi_idx++] = TWDR;
	case TWI_MRX_ADR_ACK:       // SLA+R has been transmitted and ACK received
		if (twi_idx<(twi.rxsize-1)) // Detect the last byte to NACK it.
		{
			TWCR = TW_ACK;		// send ACK after reception
		}
		else						
		{
			TWCR = TW_NAK;		// send NAK after (next) reception
		}    
		break; 
	case TWI_MRX_DATA_NACK:		// Last data byte has been received and NACK transmitted
		twi.buf[twi_idx] = TWDR; // save data byte to buffer
		twi.lastTransOK = true; // Set status bits to completed successfully. 
		TWCR = TW_STOP;			// initiate a STOP condition.
		break;      
	case TWI_ARB_LOST:          // Arbitration lost
		TWCR = TW_START;		// initiate a (RE)START condition.
		break;
	case TWI_MTX_ADR_NACK:      // SLA+W has been transmitted and NACK received
	case TWI_MRX_ADR_NACK:      // SLA+R has been transmitted and NACK received    
	case TWI_MTX_DATA_NACK:     // Data byte has been transmitted and NACK received
	case TWI_NO_STATE:          // No relevant state information available; TWINT = “0”
	case TWI_BUS_ERROR:         // Bus error due to an illegal START or STOP condition
	default:     
		twi.state = TWSR;		// Store TWSR and automatically sets noErrors bit.
		TWCR = TW_ENABLE;		// Enable TWI-interface, disable interrupts
	}
}
