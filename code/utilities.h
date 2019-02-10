/* 
 * File:   utilities.h
 * Author: Paul
 *
 * Created on February 10, 2019, 6:08 PM
 */

#ifndef UTILITIES_H
#define	UTILITIES_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <avr/io.h>
    #include "mcc_generated_files/include/pin_manager.h"
    
    typedef enum Direction {UP, RIGHT, DOWN, LEFT} Direction;

    volatile Direction last_joystick_direction;
    volatile bool joystick_flag;
    volatile bool button_flag;

    void set_led(uint8_t r, uint8_t g, uint8_t b);
    void display_4x4_block(uint8_t x, uint8_t y);
    void wait_for_button(void);


#ifdef	__cplusplus
}
#endif

#endif	/* UTILITIES_H */

