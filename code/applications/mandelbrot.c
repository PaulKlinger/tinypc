#include "breakout.h"
#include "../mcc_generated_files/config/clock_config.h"
#include <util/delay.h>
#include "../lcd.h"
#include "../utilities.h"

#include <stdfix.h>

#define init_scale 32 // pixels per unit

typedef struct {
     accum re, im;
} Complexf;

struct ScreenLoc {
    Complexf center;
    accum scale;
};

static inline Complexf csquare(Complexf z) {
    return (Complexf) {z.re * z.re - z.im * z.im, 2 * z.re * z.im};
}

static inline Complexf cadd(Complexf a, Complexf b) {
    return (Complexf) {a.re + b.re, a.im + b.im};
}

static bool point_in_set(Complexf point) {
    Complexf z = {0, 0};
    for (uint8_t i = 0; i < 20; i++){
        z = cadd(csquare(z), point);
        if (abs(z.re) > 2 || abs(z.im) > 2) {
            return false;
        }
    }
    return true;
}

Complexf screen_to_coord(uint8_t x, uint8_t y, struct ScreenLoc *screen){
    return (Complexf) {(x - (DISPLAY_WIDTH / 2.0K)) / screen->scale + screen->center.re,
                       (y - (DISPLAY_HEIGHT / 2.0K)) / screen->scale + screen->center.im};
}

static void display_mandelbrot(struct ScreenLoc *screen) {
    for (uint8_t line=0; line<DISPLAY_HEIGHT / 8; line++){
        for (uint8_t x=0; x< DISPLAY_WIDTH; x++) {
            for (uint8_t dy=0; dy < 8; dy++) {
                if (point_in_set(screen_to_coord(x, line * 8 + dy, screen))) {
                    lcd_drawPixel(x, line * 8 + dy, 1);
                }
            }
            if ((x+1)%8 == 0) {
                lcd_display_block(x-7, line, 8);
            }
        }
    }
}

void run_mandelbrot(void) {
    lcd_clear_buffer();
    struct ScreenLoc screen = {.center = {0,
                                          0},
                               .scale = init_scale};
    
    while (button_pressed);
    
    while (1) {
        lcd_clrscr();
        display_mandelbrot(&screen);
        while (1) {
            if (button_pressed) {
                uint8_t hold_down_counter = 0;
                while (button_pressed) {
                    hold_down_counter++;
                    if (hold_down_counter > 100) {
                        return;
                    }
                    _delay_ms(10);
                }
                screen.scale *= 2;
                break;
            } else if (joystick_pressed) {
                if (last_joystick_direction == LEFT) {
                    screen.center.re -= 50.0K / screen.scale;
                } else if (last_joystick_direction == RIGHT) {
                    screen.center.re += 50.0K / screen.scale;
                } else if (last_joystick_direction == UP) {
                    screen.center.im -= 25.0K / screen.scale;
                } else if (last_joystick_direction == DOWN) {
                    screen.center.im += 25.0K / screen.scale;
                }
                break;
            }
        }
        
    }
}
