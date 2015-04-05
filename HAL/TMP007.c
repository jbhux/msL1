/*************************************************** 
  This is a library for the TMP007 Temp Sensor

  Designed specifically to work with the Adafruit TMP007 Breakout 
  ----> https://www.adafruit.com/products/2023

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <stdbool.h>
#include <util/delay.h>
#include "TWI_Master.h"
#include "TMP007.h"

//#define TESTDIE 0x0C78
//#define TESTVOLT 0xFEED

uint8_t _addr;


void TMP007_init(uint8_t i2caddr) {
  _addr = i2caddr;
  TWI_Master_Initialise();
}


uint8_t TMP007_begin(uint8_t samplerate) {
  // Wire.begin();

  write16(TMP007_CONFIG, TMP007_CFG_MODEON | TMP007_CFG_ALERTEN | 
	  TMP007_CFG_TRANSC | samplerate);
  write16(TMP007_STATMASK, TMP007_STAT_ALERTEN |TMP007_STAT_CRTEN);
  // enable conversion ready alert

  uint16_t did;
  did = read16(TMP007_DEVID);
  if (did != 0x78) return false;
  return true;
}

//////////////////////////////////////////////////////

double TMP007_readDieTempC(void) {
   double Tdie = TMP007_readRawDieTemperature();
   Tdie *= 0.03125; // convert to celsius
   return Tdie;
}

double TMP007_readObjTempC(void) {
  int16_t raw = read16(TMP007_TOBJ);
  double Tobj = raw;

  // invalid
  if (raw & 0x1) Tobj = NAN;
  else {
	  raw >>=2;
	  Tobj *= 0.03125; // convert to celsius
  }
  return Tobj;
}



int16_t TMP007_readRawDieTemperature(void) {
  int16_t raw = read16(TMP007_TDIE);

  raw >>= 2;
  return raw;
}

int16_t TMP007_readRawVoltage(void) {
  int16_t raw;

  raw = read16(TMP007_VOBJ);

  return raw; 
}


/*********************************************************************/

uint16_t read16(uint8_t a) {
  uint16_t ret;
  uint8_t msg[4] = {0,0,0,0};

  TWI_ReadCombined(_addr, 1, msg, 2);
  
  while (TWI_Transceiver_Busy());
  
  if(2==TWI_Get_Data_From_Transceiver(msg, 2))   /*ok*/;
  else /*not ok*/;

  ret = msg[0]; // receive DATA
  ret <<= 8;
  ret |= msg[1]; // receive DATA

//   Wire.beginTransmission(_addr); // start transmission to device 
//   Wire.write(a); // sends register address to read from
//   Wire.endTransmission(); // end transmission
//   
//   Wire.beginTransmission(_addr); // start transmission to device 
//   Wire.requestFrom(_addr, (uint8_t)2);// send data n-bytes read
//   ret = Wire.read(); // receive DATA
//   ret <<= 8;
//   ret |= Wire.read(); // receive DATA
//   Wire.endTransmission(); // end transmission

  return ret;
}

void write16(uint8_t a, uint16_t d) {
	
	uint8_t msg[2] = {d>>8, d&0xFF};
		
	TWI_WriteCmd(_addr, a, 2, msg);
//   Wire.beginTransmission(_addr); // start transmission to device 
//   Wire.write(a); // sends register address to read from
//   Wire.write(d>>8);  // write data
//   Wire.write(d);  // write data
//   Wire.endTransmission(); // end transmission
}

