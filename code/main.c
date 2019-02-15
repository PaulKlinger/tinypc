/*
 * File:   main.c
 * Author: Paul
 *
 * Created on January 11, 2019, 7:35 PM
 */


#include "mcc_generated_files/config/clock_config.h"
#include "mcc_generated_files/mcc.h"


#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

#include "utilities.h"
#include "applications/snake.h"
#include "applications/gol.h"
#include "applications/breakout.h"
#include "applications/lander.h"
#include "applications/mandelbrot.h"


void show_launch_screen() {
    set_led(0,255,0);
    lcd_gotoxy(7,0);
    lcd_puts("~TinyPC~");
    lcd_gotoxy(3,3);
    lcd_puts("(press to start)");
    lcd_gotoxy(6,7);
    lcd_puts("by Paul Klinger");
    lcd_display();
    wait_for_button();
}

typedef struct MenuEntry {
    const char *name;
    void (*start_function)();
} MenuEntry;

typedef struct {
    uint8_t selected_index;
    uint8_t length;
    MenuEntry entries[8];
} Menu;



void display_menu(Menu *menu) {
    lcd_clear_buffer();
    for (uint8_t line=0; line < menu->length; line++) {
        lcd_gotoxy(0,line);
        if (line == menu->selected_index) {
            lcd_puts("* ");
        }
        lcd_gotoxy(2,line);
        lcd_puts(menu->entries[line].name);
    }
    lcd_display();
}

void show_menu() {
    Menu menu = {
        .length=5, .selected_index=0,
        .entries={
            {"Snake", &run_snake},
            {"Game of Life", &run_gol},
            {"Mandelbrot", &run_mandelbrot},
            {"Breakout", &run_breakout},
            {"Lander", &run_lander}
        }
    };
    display_menu(&menu);
    bool wait_for_release = true;
    while (1) {
        if (!joystick_pressed && !button_pressed) {
            wait_for_release = false;
        }
        if (joystick_pressed && !wait_for_release) {
            wait_for_release = true;
            if (last_joystick_direction == UP){
                menu.selected_index = (menu.selected_index + menu.length - 1) % menu.length;
            } else if (last_joystick_direction == DOWN) {
                menu.selected_index++;
                menu.selected_index %= menu.length;
            }
            display_menu(&menu);
        } else if (button_pressed && !wait_for_release){
            menu.entries[menu.selected_index].start_function();
            wait_for_release = true;
            display_menu(&menu);
        }
    };
    
}

int main(void) {
    SYSTEM_Initialize();
    lcd_init(LCD_DISP_ON);
    show_launch_screen();
    show_menu();
}
