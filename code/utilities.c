#include "utilities.h"

#include "light_ws2812.h"
#include "lcd.h"

void set_led(uint8_t r, uint8_t g, uint8_t b) {
    struct cRGB led[1];
    led[0].r=r;
    led[0].g=g;
    led[0].b=b;
    ws2812_setleds(led,1);
}

void display_4x4_block(uint8_t x, uint8_t y) {
    lcd_display_block(x*4, y/2, 4);
}

ISR(PORTA_PORT_vect)
{  
    /* Insert your PORTA interrupt handling code here */
    if (!IO_PA4_get_level()){
        last_joystick_direction = RIGHT;
        joystick_flag = true;
    } else if (!IO_PA5_get_level()){
        last_joystick_direction = LEFT;
        joystick_flag = true;
    } else if (!IO_PA2_get_level()) {
        last_joystick_direction = UP;
        joystick_flag = true;
    } else if (!IO_PA3_get_level()) {
        last_joystick_direction = DOWN;
        joystick_flag = true;
    }
    
    if (!IO_PA1_get_level()) {
        button_flag = true;
    }
    
    /* Clear interrupt flags */
    VPORTA.INTFLAGS = 0xff;
}

static inline bool button_pressed(){
    return !IO_PA1_get_level();
}

void wait_for_button() {
    while (!button_pressed());
}