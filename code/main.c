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
#include <math.h>

#include "lcd.h"

#include "utilities.h"
#include "applications/snake.h"
#include "applications/gol.h"


void show_launch_screen() {
    set_led(0,255,0);
    lcd_init(LCD_DISP_ON);
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
        .length=2, .selected_index=0,
        .entries={
            {"snake", &run_snake},
            {"gol", &run_gol}
        }
    };
    joystick_flag = false;
    display_menu(&menu);
    while (1) {
        if (joystick_flag) {
            if (last_joystick_direction == UP){
                menu.selected_index = (menu.selected_index + menu.length - 1) % menu.length;
            } else if (last_joystick_direction == DOWN) {
                menu.selected_index++;
                menu.selected_index %= menu.length;
            } else if (last_joystick_direction == RIGHT) {
                menu.entries[menu.selected_index].start_function();
            }
            display_menu(&menu);
            joystick_flag = false;
        }
    };
    
}

int main(void) {
    SYSTEM_Initialize();
    show_launch_screen();
    //run_snake();
    show_menu();
}
