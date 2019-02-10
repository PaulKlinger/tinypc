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

typedef struct {
    uint8_t direction_circ_buffer[128];
    uint16_t head_buffer_index;
    uint8_t head_x;
    uint8_t head_y;
    uint8_t tail_x;
    uint8_t tail_y;
    uint16_t length;
    uint8_t food_x;
    uint8_t food_y;
    Direction snake_dir;
} SnakeGamestate;


void draw_food(SnakeGamestate *gs){
    lcd_fillRect(gs->food_x*4+1, gs->food_y*4+1,
                 gs->food_x*4+2, gs->food_y*4+2,1);
}


Direction get_snake_direction(SnakeGamestate *gs, uint16_t i) {
    return (gs->direction_circ_buffer[i/4] >> (i%4)*2) & 0b00000011;
}

void set_snake_direction(SnakeGamestate *gs, uint16_t i, Direction direction) {
  gs->direction_circ_buffer[i/4] &= ~(0b00000011 << (i%4)*2); // set to zero
  gs->direction_circ_buffer[i/4] |= direction << (i%4)*2; // set to direction
}

uint16_t modulo(uint16_t x,uint16_t N){
    // correct behavior for negative x: modulo(-1, 8)=7
    return (x % N + N) %N;
}

void draw_snake(SnakeGamestate *gs) {
    uint16_t i = gs->head_buffer_index;
    uint16_t rem_length = gs->length;
    uint8_t x = gs->head_x;
    uint8_t y = gs->head_y;
    while (rem_length > 0) {
        if (rem_length == 1) {
            gs->tail_x = x;
            gs->tail_y = y;
        }
        i = modulo(i,512);
        draw_block(x, y);
        switch (get_snake_direction(gs, i)) {
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

void move_snake(SnakeGamestate *gs) {
    gs->head_buffer_index = modulo(gs->head_buffer_index + 1, 512);
    set_snake_direction(gs, gs->head_buffer_index, gs->snake_dir);
    switch (gs->snake_dir) {
        case DOWN:
            gs->head_y++;
            break;
        case UP:
            gs->head_y--;
            break;
        case LEFT:
            gs->head_x--;
            break;
        case RIGHT:
            gs->head_x++;
            break;
    }
    gs->head_x = modulo(gs->head_x, 32);
    gs->head_y = modulo(gs->head_y, 16);
}

void roll_food_position(SnakeGamestate *gs) {
    do {
        gs->food_x = rand() % 32;
        gs->food_y = rand() % 16;
    } while (lcd_check_buffer(gs->food_x*4 + 2, gs->food_y*4 + 2));
}

void set_led(uint8_t r, uint8_t g, uint8_t b) {
    struct cRGB led[1];
    led[0].r=r;
    led[0].g=g;
    led[0].b=b;
    ws2812_setleds(led,1);
}

void set_led_from_length(uint16_t length) {
    uint8_t r = fdim(200, (100.0-length)/200);
    uint8_t g = fmin(255, length*3);
    set_led(r, g, 0);
}

void display_4x4_block(uint8_t x, uint8_t y) {
    lcd_display_block(x*4, y/2, 4);
}

void reset_snake(SnakeGamestate *gs) {
    last_joystick_direction = RIGHT;
    gs->length = 1;
    gs->head_x = 0;
    gs->head_y = 0;
    gs->head_buffer_index = 0;
    set_snake_direction(gs, 0, RIGHT);
    gs->snake_dir = RIGHT;
    set_led_from_length(1);
    roll_food_position(gs);
    lcd_clear_buffer();
    draw_snake(gs);
    draw_food(gs);
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
    
    SnakeGamestate snake_state;
    reset_snake(&snake_state);
    
    while (1) {
        snake_prev_tail_x = snake_state.tail_x;
        snake_prev_tail_y = snake_state.tail_y;
        if (last_joystick_direction == RIGHT && snake_state.snake_dir != LEFT){
            snake_state.snake_dir = RIGHT;
        } else if (last_joystick_direction == LEFT && snake_state.snake_dir != RIGHT){
            snake_state.snake_dir = LEFT;
        } else if (last_joystick_direction == UP && snake_state.snake_dir != DOWN) {
            snake_state.snake_dir = UP;
        } else if (last_joystick_direction == DOWN && snake_state.snake_dir != UP) {
            snake_state.snake_dir = DOWN;
        }
        move_snake(&snake_state);
        if (snake_state.head_x == snake_state.food_x 
                && snake_state.head_y == snake_state.food_y){
            snake_state.length++;
            set_led_from_length(snake_state.length);
            roll_food_position(&snake_state);
        } else if (lcd_check_buffer(snake_state.head_x * 4, snake_state.head_y*4)) {
            // food is only 2x2 pixel, so if the top left pix is set we've crashed
            lcd_clear_buffer();
            lcd_gotoxy(6,2);
            lcd_puts("GAME OVER!\r\n");
            char points_str[3];
            itoa(snake_state.length, points_str, 10);
            lcd_gotoxy(6,3);
            lcd_puts(points_str);
            lcd_puts(" points\r\n\r\n");
            lcd_gotoxy(3,5);
            lcd_puts("(press to restart)");
            lcd_display();
            wait_for_button();
            reset_snake(&snake_state);
        }
        lcd_clear_buffer();
        draw_snake(&snake_state);
        draw_food(&snake_state);
        //lcd_display();
        display_4x4_block(snake_state.head_x, snake_state.head_y);
        display_4x4_block(snake_prev_tail_x, snake_prev_tail_y);
        // drawing the food every turn is slightly inefficient but still fast enough
        display_4x4_block(snake_state.food_x, snake_state.food_y); 
        _delay_ms(100);
    }
}
