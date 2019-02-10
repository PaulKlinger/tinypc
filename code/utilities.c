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

// All of this debouncing stuff is horribly inefficient but at least it seems to work...
// Could save a lot of ram too...
volatile static uint8_t joystick_debounce_counter; 
volatile static uint8_t button_debounce_counter;


# define DEBOUNCE_THRESHOLD 4 /* x 8ms */
ISR(RTC_PIT_vect)
{
    if (!IO_PA4_get_level()){
        last_joystick_direction = RIGHT;
    } else if (!IO_PA5_get_level()){
        last_joystick_direction = LEFT;
    } else if (!IO_PA2_get_level()) {
        last_joystick_direction = UP;
    } else if (!IO_PA3_get_level()) {
        last_joystick_direction = DOWN;
    }
    bool joystick_currently_pressed = (!IO_PA4_get_level()||!IO_PA5_get_level()
            ||!IO_PA2_get_level()||!IO_PA3_get_level());
    if (joystick_currently_pressed != joystick_pressed){
        joystick_debounce_counter++;
        if (joystick_debounce_counter >= DEBOUNCE_THRESHOLD){
            joystick_pressed = joystick_currently_pressed;
            joystick_debounce_counter = 0;
        }
    } else {
        joystick_debounce_counter = 0;
    }
    
    bool button_currently_pressed = !IO_PA1_get_level();
    if (button_currently_pressed != button_pressed) {
        button_debounce_counter++;
        if (button_debounce_counter >= DEBOUNCE_THRESHOLD) {
            button_pressed = button_currently_pressed;
            button_debounce_counter = 0;
        }
    } 
    /* TRIGB interrupt flag has to be cleared manually */
    RTC.PITINTFLAGS = RTC_PI_bm;
}


void wait_for_button() {
    while (button_pressed);
    while (!button_pressed);
}