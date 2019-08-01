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

#include "config.h"

#include "lcd.h"

#include "utilities.h"
#include "strings.h"
#include "applications/snake.h"
#include "applications/gol.h"
#include "applications/breakout.h"
#include "applications/lander.h"
#include "applications/mandelbrot.h"
#include "applications/tetris.h"
#include "applications/mines.h"


void show_launch_screen() {
    set_led(0,255,0);
    lcd_goto_xpix_y(TITLE_OFFSET,0);
    lcd_puts_p(string_title);
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
    lcd_goto_xpix_y(TITLE_OFFSET,0);
    lcd_puts_p(string_title);
    for (uint8_t i=0; i < menu->length; i++) {
        lcd_gotoxy(0,i + 1);
        if (i == menu->selected_index) {
            lcd_puts("> ");
        }
        lcd_gotoxy(2,i + 1);
        lcd_puts(menu->entries[i].name);
    }
    lcd_display();
}

void show_menu() {
    Menu menu = {
        .length=6, .selected_index=0,
        .entries={
            {"Snake", &run_snake},
            //{"Breakout", &run_breakout},
            {"Tetris", &run_tetris},
            {"Lander", &run_lander},
            {"Mines", &run_mines},
            {"Game of Life", &run_gol},
            {"Mandelbrot", &run_mandelbrot}
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
    srand((unsigned int) RTC.CNT);
    show_menu();
}
