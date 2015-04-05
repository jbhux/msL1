/*
 * adc.c
 *
 * Created: 25.03.2015 13:26:18
 *  Author: sebastian.elber
 */ 
#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "HAL_MobiRob.h"
//#include "io_port.h"


// General ADC-Variables
uint8_t ADC_REF = 0;

// Read the AD-Conversion Result
uint16_t adc_read(uint8_t adc_input)
{
	uint16_t result = 0;
	
	// Multiplexer-Setup for lower 8 ADC-Inputs (0 - 7)
	ADMUX = (adc_input & 0x1F)|ADC_REF;
	
	// Multiplexer-Setup for upper 8 ADC-Inputs (8 - 15)
	if(adc_input & 0x20)ADCSRB |= (1<<MUX5);
	else ADCSRB &= ~(1<<MUX5);
	
	// Stabilization of the ADC Input-Voltage
	_delay_us(10);
	
	// Start Conversion...
	ADCSRA |= (1<<ADSC);
	
	// Wait for the Conversion to complete
	while (!(ADCSRA & (1<<ADIF)));
	
	// Reset the Conversion-Completed Flag
	ADCSRA |= (1<<ADIF);
	
	// Calculate the Conversion Result
	result=ADC;	
	
	// Return the Conversion Result
	return result;
}



void adc_init(uint8_t adc_reference)
{
#define F_ADC (200000L)
#define	PRESC (F_CPU/F_ADC)
	
#if (PRESC>64)
	#define ADPS (7)
#elif (PRESC>32)	
	#define ADPS (6)
#elif (PRESC>16)
	#define ADPS (5)
#elif (PRESC>8)	
	#define ADPS (4)
#elif (PRESC>4)	
	#define ADPS (3)
#elif (PRESC>2)	
	#define ADPS (2)
#else
	#define ADPS (1)
#endif
	
	// Analog Comparator Control and Status Register
	ACSR = (1<<ACD)|(0<<ACBG)|(0<<ACO)|(0<<ACI)|(0<<ACIE)|(0<<ACIC)|(0<<ACIS1)|(0<<ACIS0);

	// Digital Input Disable Register 1 -> Analog Comparator Inputs AIN0 & AIN1  (1 = disabled, 0 = enabled)
	DIDR1 = (1<<AIN0D)|(1<<AIN1D);

	// Digital Input Disable Register 0 -> Analog/Digital Converter ADC0 - ADC7  (1 = disabled, 0 = enabled)
	//DIDR0 = (1<<ADC7D)|(1<<ADC6D)|(1<<ADC5D)|(1<<ADC4D)|(1<<ADC3D)|(1<<ADC2D)|(1<<ADC1D)|(1<<ADC0D);
	DIDR0 = ((ANALOG_IR_4<<ADC3D)|(ANALOG_IR_3<<ADC2D)|(ANALOG_IR_2<<ADC1D)|(ANALOG_IR_1<<ADC0D));

	// Digital Input Disable Register 2 -> Analog/Digital Converter ADC8 - ADC15 (1 = disabled, 0 = enabled)
	//DIDR2 = (0<<ADC15D)|(0<<ADC14D)|(0<<ADC13D)|(0<<ADC12D)|(0<<ADC11D)|(0<<ADC10D)|(0<<ADC9D)|(0<<ADC8D);
	DIDR2 = ((ANALOG_ADC_15<<ADC15D)|(ANALOG_ADC_14<<ADC14D)|(ANALOG_LINE_4<<ADC13D)|(ANALOG_LINE_3<<ADC12D)|
			(ANALOG_LINE_2<<ADC11D)|(ANALOG_LINE_1<<ADC10D)|(ANALOG_MAG_1<<ADC9D)|(ANALOG_BATT<<ADC8D));
	
	// Initialization...
	ADC_REF = 0;
	
	// Select the ADC-Reference -> Result Right-Adjusted
	switch(adc_reference)
	{
		case VREF:		ADC_REF = ((0<<REFS1)|(0<<REFS0)|(0<<ADLAR));
		break;
		case AVCC:		ADC_REF = ((0<<REFS1)|(1<<REFS0)|(0<<ADLAR));
		break;
		case REF_1V1:	ADC_REF = ((1<<REFS1)|(0<<REFS0)|(0<<ADLAR));
		break;
		case REF_2V56:
		default:		ADC_REF = ((1<<REFS1)|(1<<REFS0)|(0<<ADLAR));
		break;
	}
	ADMUX = ADC_REF;
	
	// ADC Control and Status Register A -> ADC-enabled! Auto Trigger Enable cleared, Prescaler for fADC <= 200kHz
	// ADCSRA = (1<<ADEN)|(0<<ADSC)|(1<<ADATE)|(0<<ADIF)|(0<<ADIE)|(0<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	ADCSRA = (1<<ADEN)|(0<<ADSC)|(0<<ADATE)|(1<<ADIF)|(0<<ADIE)|ADPS;
	
	// ADC Control and Status Register B -> Free Running ADC Mode
	ADCSRB = (0<<MUX5)|(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0);
}