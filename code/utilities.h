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
    #include <stdfix.h>
    
    typedef enum Direction {UP, RIGHT, DOWN, LEFT} Direction;
    
    typedef struct {accum x, y;} AccVec;

    volatile Direction last_joystick_direction;
    volatile bool joystick_pressed;
    volatile bool button_pressed;

    void set_led(uint8_t r, uint8_t g, uint8_t b);
    void display_4x4_block(uint8_t x, uint8_t y);
    void wait_for_button(void);
    void show_game_over_screen(uint8_t points);
    void set_led_from_points(uint16_t points, uint16_t max_points);
    
    static inline int8_t modulo(int8_t a, int8_t b){
        // correctly handle negative values
        return (a%b+b)%b;
    }
    
    accum roundacc0(accum x);
    uint8_t ceilacc8(accum x);
    
    void rotate_vec(AccVec *vec, int8_t angle);


#ifdef	__cplusplus
}
#endif

#endif	/* UTILITIES_H */

