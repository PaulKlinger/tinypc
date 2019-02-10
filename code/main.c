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


int main(void) {
    SYSTEM_Initialize();
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
    run_snake();
}
