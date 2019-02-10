/*
 * File:   main.c
 * Author: Paul
 *
 * Created on January 11, 2019, 7:35 PM
 */


#include "mcc_generated_files/config/clock_config.h"
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/include/pin_manager.h"

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#include "ws2812_config.h"
#include "light_ws2812.h"


#include "lcd.h"


void draw_block(uint8_t x, uint8_t y) {
    lcd_fillRect(x*4, y*4, x*4+3, y*4+3,1);
}

typedef enum Direction {UP, RIGHT, DOWN, LEFT} Direction;

static volatile Direction last_joystick_direction;

static uint8_t snake_direction_circ_buffer[128];
static uint16_t snake_head_buffer_index;
static uint8_t snake_head_x;
static uint8_t snake_head_y;
static uint8_t snake_tail_x;
static uint8_t snake_tail_y;
static uint16_t snake_length;
static uint8_t food_x;
static uint8_t food_y;
static Direction snake_dir;

void draw_food(){
    lcd_fillRect(food_x*4+1, food_y*4+1, food_x*4+2, food_y*4+2,1);
}


Direction get_snake_direction(uint16_t i) {
    return (snake_direction_circ_buffer[i/4] >> (i%4)*2) & 0b00000011;
}

void set_snake_direction(uint16_t i, Direction direction) {
  snake_direction_circ_buffer[i/4] &= ~(0b00000011 << (i%4)*2); // set to zero
  snake_direction_circ_buffer[i/4] |= direction << (i%4)*2; // set to direction
}

uint16_t modulo(uint16_t x,uint16_t N){
    // correct behavior for negative x: modulo(-1, 8)=7
    return (x % N + N) %N;
}

void draw_snake() {
    uint16_t i = snake_head_buffer_index;
    uint16_t rem_length = snake_length;
    uint8_t x = snake_head_x;
    uint8_t y = snake_head_y;
    while (rem_length > 0) {
        if (rem_length == 1) {
            snake_tail_x = x;
            snake_tail_y = y;
        }
        i = modulo(i,512);
        draw_block(x, y);
        switch (get_snake_direction(i)) {
            // opposite directions because we are going backwards
            case UP: {y++;break;};
            case DOWN: {y--;break;};
            case LEFT: {x++;break;};
            case RIGHT: {x--;break;};
        }
        y = modulo(y,16);
        x = modulo(x,32);
        i--;
        rem_length--;
    }
}

void move_snake(Direction dir) {
    snake_head_buffer_index = modulo(snake_head_buffer_index + 1, 512);
    set_snake_direction(snake_head_buffer_index, dir);
    switch (dir) {
        case DOWN:
            snake_head_y++;
            break;
        case UP:
            snake_head_y--;
            break;
        case LEFT:
            snake_head_x--;
            break;
        case RIGHT:
            snake_head_x++;
            break;
    }
    snake_head_x = modulo(snake_head_x, 32);
    snake_head_y = modulo(snake_head_y, 16);
}

void roll_food_position() {
    do {
        food_x = rand() % 32;
        food_y = rand() % 16;
    } while (lcd_check_buffer(food_x*4 + 2, food_y*4 + 2));
}

void set_led(uint8_t r, uint8_t g, uint8_t b) {
    struct cRGB led[1];
    led[0].r=r;
    led[0].g=g;
    led[0].b=b;
    ws2812_setleds(led,1);
}

void set_led_from_length(uint16_t length) {
    uint8_t r = fdim(200, (100.0-snake_length)/200);
    uint8_t g = fmin(255, snake_length*3);
    set_led(r, g, 0);
}

void display_4x4_block(uint8_t x, uint8_t y) {
    lcd_display_block(x*4, y/2, 4);
}

void reset_snake() {
    last_joystick_direction = RIGHT;
    set_led(200,0,0);
    snake_length = 1;
    snake_head_x = 0;
    snake_head_y = 0;
    snake_head_buffer_index = 0;
    set_snake_direction(0, RIGHT);
    snake_dir = RIGHT;
    roll_food_position();
    lcd_clear_buffer();
    draw_snake();
    draw_food();
    lcd_display();
}


ISR(PORTA_PORT_vect)
{  
    /* Insert your PORTA interrupt handling code here */
    if (!IO_PA4_get_level()){
        last_joystick_direction = RIGHT;
    } else if (!IO_PA5_get_level()){
        last_joystick_direction = LEFT;
    } else if (!IO_PA2_get_level()) {
        last_joystick_direction = UP;
    } else if (!IO_PA3_get_level()) {
        last_joystick_direction = DOWN;
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
    
    uint8_t snake_prev_tail_x;
    uint8_t snake_prev_tail_y;
    
    reset_snake();
    
    while (1) {
        snake_prev_tail_x = snake_tail_x;
        snake_prev_tail_y = snake_tail_y;
        if (last_joystick_direction == RIGHT && snake_dir != LEFT){
            snake_dir = RIGHT;
        } else if (last_joystick_direction == LEFT && snake_dir != RIGHT){
            snake_dir = LEFT;
        } else if (last_joystick_direction == UP && snake_dir != DOWN) {
            snake_dir = UP;
        } else if (last_joystick_direction == DOWN && snake_dir != UP) {
            snake_dir = DOWN;
        }
        move_snake(snake_dir);
        if (snake_head_x == food_x && snake_head_y == food_y){
            snake_length++;
            set_led_from_length(snake_length);
            roll_food_position();
        } else if (lcd_check_buffer(snake_head_x * 4, snake_head_y*4)) {
            // food is only 2x2 pixel, so if the top left pix is set we've crashed
            lcd_clear_buffer();
            lcd_gotoxy(6,2);
            lcd_puts("GAME OVER!\r\n");
            char points_str[3];
            itoa(snake_length, points_str, 10);
            lcd_gotoxy(6,3);
            lcd_puts(points_str);
            lcd_puts(" points\r\n\r\n");
            lcd_gotoxy(3,5);
            lcd_puts("(press to restart)");
            lcd_display();
            wait_for_button();
            reset_snake();
        }
        lcd_clear_buffer();
        draw_snake();
        draw_food();
        //lcd_display();
        display_4x4_block(snake_head_x, snake_head_y);
        display_4x4_block(snake_prev_tail_x, snake_prev_tail_y);
        // drawing the food every turn is slightly inefficient but still fast enough
        display_4x4_block(food_x, food_y); 
        _delay_ms(100);
    }
}
