/* 
 * File:   strings.h
 * Author: Paul
 *
 * Created on February 15, 2019, 11:56 PM
 */

#ifndef STRINGS_H
#define	STRINGS_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <avr/pgmspace.h>
    
static const char string_next_stage[] PROGMEM = "next stage";
static const char string_press_to_return[] PROGMEM = "(press to return)";
static const char string_game_over[] PROGMEM = "GAME OVER";
static const char string_points[] PROGMEM = " points";
#ifdef	__cplusplus
}
#endif

#endif	/* STRINGS_H */

