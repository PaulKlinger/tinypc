#include "breakout.h"
#include "../mcc_generated_files/config/clock_config.h"
#include <util/delay.h>
#include "../lcd.h"
#include "../utilities.h"

#include <stdfix.h>
#include <string.h>

#define init_scale 32 // pixels per unit

struct ScreenLoc {
    AccVec center;
    accum scale;
};


static bool point_in_set(AccVec point, uint8_t maxiter) {
    accum z_re = 0;
    accum z_im = 0;
    accum z_re_sq;
    accum z_im_sq;
    for (uint8_t i = 0; i < maxiter; i++){
        z_re_sq = z_re * z_re;
        z_im_sq = z_im * z_im;
        if (z_re_sq + z_im_sq > 4) {
            return false;
        }
        z_im = 2 * z_re * z_im + point.y;
        z_re = z_re_sq - z_im_sq + point.x;
    }
    return true;
}

static AccVec screen_to_coord(uint8_t x, uint8_t y, struct ScreenLoc *screen){
    return (AccVec) {(x - (DISPLAY_WIDTH / 2)) / screen->scale + screen->center.x,
                       (y - (DISPLAY_HEIGHT / 2)) / screen->scale + screen->center.y};
}

static uint8_t get_maxiter(struct ScreenLoc *screen) {
    if (screen->scale <= init_scale * 4) return 20;
    if (screen->scale <= init_scale * 8) return 30;
    return 40;
}

static void display_mandelbrot_block(struct ScreenLoc *screen, uint8_t x, uint8_t xmax, uint8_t line) {
    for (; x <= xmax; x++) {
        for (uint8_t dy=0; dy < 8; dy++) {
            if (point_in_set(screen_to_coord(x, line * 8 + dy, screen), get_maxiter(screen))) {
                lcd_drawPixel(x, line * 8 + dy, 1);
            }
        }
        if ((x+1)%8 == 0) {
            lcd_display_block(x-7, line, 8);
        }
    }
    
}

static void display_complete_mandelbrot(struct ScreenLoc *screen) {
    lcd_clrscr();
    for (uint8_t line=0; line<DISPLAY_HEIGHT / 8; line++){
            display_mandelbrot_block(screen, 0, DISPLAY_WIDTH - 1, line);
    }
}


// There probably is some way to integrate these into vertical/horizontal
// functions, but I'm too tired to figure it out...
static void move_up(struct ScreenLoc *screen){
    screen->center.y -= 16 / screen->scale;
    for (uint8_t line = DISPLAY_HEIGHT / 8 - 1; line > 1; line--) {
        memcpy(displayBuffer[line], displayBuffer[line-2], DISPLAY_WIDTH);
    }
    memset(displayBuffer, 0, 2*DISPLAY_WIDTH);
    lcd_display();
    display_mandelbrot_block(screen, 0, DISPLAY_WIDTH - 1, 1);
    display_mandelbrot_block(screen, 0, DISPLAY_WIDTH - 1, 0);
}

static void move_down(struct ScreenLoc *screen){
    screen->center.y += 16 / screen->scale;
    for (uint8_t line = 0; line < DISPLAY_HEIGHT / 8 - 2; line++) {
        memcpy(displayBuffer[line], displayBuffer[line+2], DISPLAY_WIDTH);
    }
    memset(displayBuffer[DISPLAY_HEIGHT / 8 - 2], 0, 2*DISPLAY_WIDTH);
    lcd_display();
    display_mandelbrot_block(screen, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT / 8 - 2);
    display_mandelbrot_block(screen, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT / 8 - 1);
}

static void move_left(struct ScreenLoc *screen){
    screen->center.x -= 32 / screen->scale;
    for (uint8_t line = 0; line < DISPLAY_HEIGHT / 8; line++) {
        memmove(displayBuffer[line] + 32, displayBuffer[line], DISPLAY_WIDTH - 32);
        memset(displayBuffer[line], 0, 32);
    }
    lcd_display();
    for (uint8_t line = 0; line < DISPLAY_HEIGHT / 8; line++) {
        display_mandelbrot_block(screen, 0, 32, line);
    }
}

static void move_right(struct ScreenLoc *screen){
    screen->center.x += 32 / screen->scale;
    for (uint8_t line = 0; line < DISPLAY_HEIGHT / 8; line++) {
        memmove(displayBuffer[line], displayBuffer[line] + 32, DISPLAY_WIDTH - 32);
        memset(displayBuffer[line] + (DISPLAY_WIDTH - 32), 0, 32);
    }
    lcd_display();
    for (uint8_t line = 0; line < DISPLAY_HEIGHT / 8; line++) {
        display_mandelbrot_block(screen, DISPLAY_WIDTH - 33, DISPLAY_WIDTH - 1, line);
    }
}


void run_mandelbrot(void) {
    lcd_clear_buffer();
    struct ScreenLoc screen = {.center = {0,
                                          0},
                               .scale = init_scale};
    
    while (button_pressed);
    display_complete_mandelbrot(&screen);
    
    while (1) {
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
                display_complete_mandelbrot(&screen);
                break;
            } else if (joystick_pressed) {
                switch (last_joystick_direction) {
                    case LEFT:
                        move_left(&screen);
                        break;
                    case RIGHT:
                        move_right(&screen);
                        break;
                    case UP:
                        move_up(&screen);
                        break;
                    case DOWN:
                        move_down(&screen);
                        break;
                }
                break;
            }
        }
        
    }
}
