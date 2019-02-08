/*
 * light_ws2812_config.h
 *
 * v2.4 - Nov 27, 2016
 * 2019-02-07 - Modified by Paul Klinger to work with Attiny 1 series MCUs
 *
 * User Configuration file for the light_ws2812_lib
 *
 */ 


#ifndef WS2812_CONFIG_H_
#define WS2812_CONFIG_H_

///////////////////////////////////////////////////////////////////////
// Enable if using an Attiny 1 series MCU
// (they use different configuration registers)
///////////////////////////////////////////////////////////////////////
#define ws2812_Attiny1

///////////////////////////////////////////////////////////////////////
// Define Reset time in µs. 
//
// This is the time the library spends waiting after writing the data.
//
// WS2813 needs 300 µs reset time
// WS2812 and clones only need 50 µs
//
///////////////////////////////////////////////////////////////////////

#define ws2812_resettime  300 

///////////////////////////////////////////////////////////////////////
// Define I/O pin
///////////////////////////////////////////////////////////////////////


#define ws2812_port B     // Data port 
#define ws2812_pin  2     // Data out pin

#endif /* WS2812_CONFIG_H_ */