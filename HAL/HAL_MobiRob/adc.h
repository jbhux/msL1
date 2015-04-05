/*
 * adc.h
 *
 * Created: 25.03.2015 13:27:22
 *  Author: sebastian.elber
 */ 


#ifndef ADC_H_
#define ADC_H_

// General ADC-State Enumeration
enum
{
	// selectable ADC-References
	VREF,
	AVCC,
	REF_1V1,
	REF_2V56,
	
	// selectable ADC-Resolution
	ADC_10BIT,
	ADC_8BIT
};

// Do a single AD-Conversion, either with 10Bit or 8Bit Resolution
uint16_t adc_read(uint8_t adc_input, uint8_t adc_precision);

// Initialize the AD-Converter
void adc_init(uint8_t adc_reference);

#endif /* ADC_H_ */