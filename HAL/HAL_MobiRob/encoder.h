/*
 * encoder.h
 *
 * Created: 20.03.2015 13:00:42
 *  Author: daniel
 */ 


#ifndef ENCODER_H_
#define ENCODER_H_

#if USE_ENC_2 > 0
#if USE_PINCHANGE_PJ4 > 0
#error double use of PCINT13. PCINT13 is reserved by USE_ENC_2.
#else
#undef USE_PINCHANGE_PJ4
#define USE_PINCHANGE_PJ4 (1)
#endif
#endif
#if USE_ENC_1 > 0
#if USE_PINCHANGE_PJ5 > 0
#error double use of PCINT14. PCINT14 is reserved by USE_ENC_1.
#else
#undef USE_PINCHANGE_PJ5
#define USE_PINCHANGE_PJ5 (1)
#endif
#endif




#endif /* ENCODER_H_ */