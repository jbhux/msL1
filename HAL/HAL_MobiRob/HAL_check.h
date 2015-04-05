/*
 * HAL_config.h
 *
 * Created: 24.03.2015 12:50:17
 * Author: daniel.binggeli
 */ 


#ifndef HAL_CHECK_H_
#define HAL_CHECK_H_


// HINWEIS:
// ------------------------------------------
// gültige Werte für die USE_xy-Flag sind nur (0)
// und (1). Alle anderen Werte führen zu Fehlern.

#if(((USE_MOTOR_1)>1)||((USE_MOTOR_1)<0))	
#error in HAL_config.h: Wrong value for USE_MOTOR_1 defined. Define USE_MOTOR_1 either (0) or (1).
#endif
#if(((USE_MOTOR_2)>1)||((USE_MOTOR_2)<0))
#error in HAL_config.h: Wrong value for USE_MOTOR_2 defined. Define USE_MOTOR_2 either (0) or (1).
#endif
#if(((USE_MOTOR_3)>1)||((USE_MOTOR_3)<0))
#error in HAL_config.h: Wrong value for USE_MOTOR_3 defined. Define USE_MOTOR_3 either (0) or (1).
#endif
#if(((USE_MOTOR_4)>1)||((USE_MOTOR_4)<0))
#error in HAL_config.h: Wrong value for USE_MOTOR_4 defined. Define USE_MOTOR_4 either (0) or (1).
#endif
#if(((USE_SERVO_0)>1)||((USE_SERVO_0)<0))
#error in HAL_config.h: Wrong value for USE_SERVO_0 defined. Define USE_SERVO_0 either (0) or (1).
#endif
#if(((USE_SERVO_1)>1)||((USE_SERVO_1)<0))	
#error in HAL_config.h: Wrong value for USE_SERVO_1 defined. Define USE_SERVO_1 either (0) or (1).
#endif
#if(((USE_SERVO_2)>1)||((USE_SERVO_2)<0))	
#error in HAL_config.h: Wrong value for USE_SERVO_2 defined. Define USE_SERVO_2 either (0) or (1).
#endif
#if(((USE_SERVO_3)>1)||((USE_SERVO_3)<0))	
#error in HAL_config.h: Wrong value for USE_SERVO_3 defined. Define USE_SERVO_3 either (0) or (1).
#endif
#if(((USE_SERVO_4)>1)||((USE_SERVO_4)<0))	
#error in HAL_config.h: Wrong value for USE_SERVO_4 defined. Define USE_SERVO_4 either (0) or (1).
#endif
#if(((USE_SERVO_5)>1)||((USE_SERVO_5)<0))	
#error in HAL_config.h: Wrong value for USE_SERVO_5 defined. Define USE_SERVO_5 either (0) or (1).
#endif
#if(((USE_SERVO_6)>1)||((USE_SERVO_6)<0))	
#error in HAL_config.h: Wrong value for USE_SERVO_6 defined. Define USE_SERVO_6 either (0) or (1).
#endif
#if(((USE_SERVO_7)>1)||((USE_SERVO_7)<0))	
#error in HAL_config.h: Wrong value for USE_SERVO_7 defined. Define USE_SERVO_7 either (0) or (1).
#endif
#if(((USE_SERVO_8)>1)||((USE_SERVO_8)<0))	
#error in HAL_config.h: Wrong value for USE_SERVO_8 defined. Define USE_SERVO_8 either (0) or (1).
#endif
#if(((USE_SERVO_9)>1)||((USE_SERVO_9)<0))	
#error in HAL_config.h: Wrong value for USE_SERVO_9 defined. Define USE_SERVO_9 either (0) or (1).
#endif
#if(((USE_SONIC_1)>1)||((USE_SONIC_1)<0))	
#error in HAL_config.h: Wrong value for USE_SONIC_1 defined. Define USE_SONIC_1 either (0) or (1).
#endif
#if(((USE_SONIC_2)>1)||((USE_SONIC_2)<0))	
#error in HAL_config.h: Wrong value for USE_SONIC_2 defined. Define USE_SONIC_2 either (0) or (1).
#endif
#if(((USE_BATT)>1)||((USE_BATT)<0))
#error in HAL_config.h: Wrong value for USE_BATT defined. Define USE_BATT either (0) or (1).
#endif
#if(((USE_MAG_1)>1)||((USE_MAG_1)<0))
#error in HAL_config.h: Wrong value for USE_MAG_1 defined. Define USE_MAG_1 either (0) or (1).
#endif
#if(((USE_LINE_1)>1)||((USE_LINE_1)<0))	
#error in HAL_config.h: Wrong value for USE_LINE_1 defined. Define USE_LINE_1 either (0) or (1).
#endif
#if(((USE_LINE_2)>1)||((USE_LINE_2)<0))	
#error in HAL_config.h: Wrong value for USE_LINE_2 defined. Define USE_LINE_2 either (0) or (1).
#endif
#if(((USE_LINE_3)>1)||((USE_LINE_3)<0))	
#error in HAL_config.h: Wrong value for USE_LINE_3 defined. Define USE_LINE_3 either (0) or (1).
#endif
#if(((USE_LINE_4)>1)||((USE_LINE_4)<0))	
#error in HAL_config.h: Wrong value for USE_LINE_4 defined. Define USE_LINE_4 either (0) or (1).
#endif
#if(((USE_IR_1)>1)||((USE_IR_1)<0))	
#error in HAL_config.h: Wrong value for USE_IR_1 defined. Define USE_IR_1 either (0) or (1).
#endif
#if(((USE_IR_2)>1)||((USE_IR_2)<0))	
#error in HAL_config.h: Wrong value for USE_IR_2 defined. Define USE_IR_2 either (0) or (1).
#endif
#if(((USE_IR_3)>1)||((USE_IR_3)<0))	
#error in HAL_config.h: Wrong value for USE_IR_3 defined. Define USE_IR_3 either (0) or (1).
#endif
#if(((USE_IR_4)>1)||((USE_IR_4)<0))	
#error in HAL_config.h: Wrong value for USE_IR_4 defined. Define USE_IR_4 either (0) or (1).
#endif
#if(((USE_ENC_1)>1)||((USE_ENC_1)<0))	
#error in HAL_config.h: Wrong value for USE_ENC_1 defined. Define USE_ENC_1 either (0) or (1).
#endif
#if(((USE_ENC_2)>1)||((USE_ENC_2)<0))	
#error in HAL_config.h: Wrong value for USE_ENC_2 defined. Define USE_ENC_2 either (0) or (1).
#endif
#if(((USE_QUADDEC_1)>1)||((USE_QUADDEC_1)<0))	
#error in HAL_config.h: Wrong value for USE_QUADDEC_1 defined. Define USE_QUADDEC_1 either (0) or (1).
#endif
#if(((USE_QUADDEC_2)>1)||((USE_QUADDEC_2)<0))	
#error in HAL_config.h: Wrong value for USE_QUADDEC_2 defined. Define USE_QUADDEC_2 either (0) or (1).
#endif
#if(((USE_SPI)>1)||((USE_SPI)<0))	
#error in HAL_config.h: Wrong value for USE_SPI defined. Define USE_SPI either (0) or (1).
#endif
#if(((USE_UART0)>1)||((USE_UART0)<0))	
#error in HAL_config.h: Wrong value for USE_UART0 defined. Define USE_UART0 either (0) or (1).
#endif
#if(((USE_UART2)>1)||((USE_UART2)<0))	
#error in HAL_config.h: Wrong value for USE_UART2 defined. Define USE_UART2 either (0) or (1).
#endif

#if(((USE_SYSTEM_TICK)>1)||((USE_SYSTEM_TICK)<0))	
#error in HAL_config.h: Wrong value for USE_SYSTEM_TICK defined. Define USE_SYSTEM_TICK either (0) or (1).
#endif
#if(((USE_PINCHANGE_PB0)>1)||((USE_PINCHANGE_PB0)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PB0 defined. Define USE_PINCHANGE_PB0 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PB1)>1)||((USE_PINCHANGE_PB1)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PB1 defined. Define USE_PINCHANGE_PB1 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PB2)>1)||((USE_PINCHANGE_PB2)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PB2 defined. Define USE_PINCHANGE_PB2 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PB3)>1)||((USE_PINCHANGE_PB3)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PB3 defined. Define USE_PINCHANGE_PB3 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PB4)>1)||((USE_PINCHANGE_PB4)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PB4 defined. Define USE_PINCHANGE_PB4 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PB5)>1)||((USE_PINCHANGE_PB5)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PB5 defined. Define USE_PINCHANGE_PB5 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PB6)>1)||((USE_PINCHANGE_PB6)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PB6 defined. Define USE_PINCHANGE_PB6 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PB7)>1)||((USE_PINCHANGE_PB7)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PB7 defined. Define USE_PINCHANGE_PB7 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PE0)>1)||((USE_PINCHANGE_PE0)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PE0 defined. Define USE_PINCHANGE_PE0 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PJ0)>1)||((USE_PINCHANGE_PJ0)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PJ0 defined. Define USE_PINCHANGE_PJ0 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PJ1)>1)||((USE_PINCHANGE_PJ1)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PJ1 defined. Define USE_PINCHANGE_PJ1 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PJ2)>1)||((USE_PINCHANGE_PJ2)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PJ2 defined. Define USE_PINCHANGE_PJ2 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PJ3)>1)||((USE_PINCHANGE_PJ3)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PJ3 defined. Define USE_PINCHANGE_PJ3 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PJ4)>1)||((USE_PINCHANGE_PJ4)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PJ4 defined. Define USE_PINCHANGE_PJ4 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PJ5)>1)||((USE_PINCHANGE_PJ5)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PJ5 defined. Define USE_PINCHANGE_PJ5 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PJ6)>1)||((USE_PINCHANGE_PJ6)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PJ6 defined. Define USE_PINCHANGE_PJ6 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PK0)>1)||((USE_PINCHANGE_PK0)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PK0 defined. Define USE_PINCHANGE_PK0 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PK1)>1)||((USE_PINCHANGE_PK1)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PK1 defined. Define USE_PINCHANGE_PK1 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PK2)>1)||((USE_PINCHANGE_PK2)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PK2 defined. Define USE_PINCHANGE_PK2 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PK3)>1)||((USE_PINCHANGE_PK3)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PK3 defined. Define USE_PINCHANGE_PK3 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PK4)>1)||((USE_PINCHANGE_PK4)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PK4 defined. Define USE_PINCHANGE_PK4 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PK5)>1)||((USE_PINCHANGE_PK5)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PK5 defined. Define USE_PINCHANGE_PK5 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PK6)>1)||((USE_PINCHANGE_PK6)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PK6 defined. Define USE_PINCHANGE_PK6 either (0) or (1).
#endif
#if(((USE_PINCHANGE_PK7)>1)||((USE_PINCHANGE_PK7)<0))	
#error in HAL_config.h: Wrong value for USE_PINCHANGE_PK7 defined. Define USE_PINCHANGE_PK7 either (0) or (1).
#endif
#if(((ANALOG_IR_1)>1)||((ANALOG_IR_1)<0)) 
#error in HAL_config.h: Wrong value for ANALOG_IR_1 defined. Define ANALOG_IR_1 either (0) or (1). 
#endif
#if(((ANALOG_IR_2)>1)||((ANALOG_IR_2)<0)) 
#error in HAL_config.h: Wrong value for ANALOG_IR_2 defined. Define ANALOG_IR_2 either (0) or (1). 
#endif
#if(((ANALOG_IR_3)>1)||((ANALOG_IR_3)<0)) 
#error in HAL_config.h: Wrong value for ANALOG_IR_3 defined. Define ANALOG_IR_3 either (0) or (1). 
#endif
#if(((ANALOG_IR_4)>1)||((ANALOG_IR_4)<0)) 
#error in HAL_config.h: Wrong value for ANALOG_IR_4 defined. Define ANALOG_IR_4 either (0) or (1). 
#endif
#if(((ANALOG_BATT)>1)||((ANALOG_BATT)<0)) 
#error in HAL_config.h: Wrong value for ANALOG_BATT defined. Define ANALOG_BATT either (0) or (1). 
#endif
#if(((ANALOG_MAG_1)>1)||((ANALOG_MAG_1)<0)) 
#error in HAL_config.h: Wrong value for ANALOG_MAG_1 defined. Define ANALOG_MAG_1 either (0) or (1). 
#endif
#if(((ANALOG_LINE_1)>1)||((ANALOG_LINE_1)<0)) 
#error in HAL_config.h: Wrong value for ANALOG_LINE_1 defined. Define ANALOG_LINE_1 either (0) or (1). 
#endif
#if(((ANALOG_LINE_2)>1)||((ANALOG_LINE_2)<0))
#error in HAL_config.h: Wrong value for ANALOG_LINE_2 defined. Define ANALOG_LINE_2 either (0) or (1). 
#endif
#if(((ANALOG_LINE_3)>1)||((ANALOG_LINE_3)<0)) 
#error in HAL_config.h: Wrong value for ANALOG_LINE_3 defined. Define ANALOG_LINE_3 either (0) or (1). 
#endif
#if(((ANALOG_LINE_4)>1)||((ANALOG_LINE_4)<0))
#error in HAL_config.h: Wrong value for ANALOG_LINE_4 defined. Define ANALOG_LINE_4 either (0) or (1).
#endif
#if(((ANALOG_ADC_14)>1)||((ANALOG_ADC_14)<0))
#error in HAL_config.h: Wrong value for ANALOG_ADC_14 defined. Define ANALOG_ADC_14 either (0) or (1).
#endif
#if(((ANALOG_ADC_15)>1)||((ANALOG_ADC_15)<0))
#error in HAL_config.h: Wrong value for ANALOG_ADC_15 defined. Define ANALOG_ADC_15 either (0) or (1).
#endif


#endif /* HAL_CHECK_H_ */