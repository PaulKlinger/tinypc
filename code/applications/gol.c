#include "../mcc_generated_files/config/clock_config.h"
#include <avr/io.h>
#include <util/delay.h>

#include "../lcd.h"
#include "../utilities.h"

#define height 32
#define width 64
#define byte_width 8

bool get_cell(uint8_t board[height][byte_width], uint8_t x, uint8_t y) {
    return board[y][x / 8] & (1 << (x % 8));
}

bool set_cell(uint8_t board[height][byte_width], uint8_t x, uint8_t y) {
    return board[y][x / 8] |= (1 << (x % 8));
}

bool unset_cell(uint8_t board[height][byte_width], uint8_t x, uint8_t y) {
    return board[y][x / 8] &= ~(1 << (x % 8));
}

bool get_cell_from_buffer(uint8_t x, uint8_t y) {
    return lcd_check_buffer(x*2, y*2);
}

void draw_cell(uint8_t x, uint8_t y) {
    lcd_fillRect(x*2, y*2, x*2+1, y*2+1, 1);
}

void display_board(uint8_t board[height][byte_width]) {
    lcd_clear_buffer();
    for (uint8_t y=0; y<height;y++){
        for (uint8_t x=0; x<width;x++){
            if (get_cell(board, x, y)){
                draw_cell(x,y);
            }
        };
    };
    lcd_display();
}

int8_t modulo(int8_t a, int8_t b){
    return (a%b+b)%b;
}

void update_board(uint8_t board[height][byte_width]) {
    uint8_t neighbors;
    for (int8_t y=0; y<height;y++){
        for (int8_t x=0; x<width;x++){
            neighbors = 0;
            for (int8_t dx=-1; dx<=1; dx++) {
                for (int8_t dy=-1; dy<=1; dy++) {
                    if (!(dy==0 && dx==0)
                            && get_cell_from_buffer(
                            modulo(x+dx,width),
                            modulo(y+dy,height))){
                        neighbors++;
                    }
                }
            }
            if (neighbors > 3 || neighbors < 2) {
                unset_cell(board, x, y);
            } else if (neighbors == 3) {
                set_cell(board, x, y);
            }
        };
    };
}

void run_gol() {
    srand((unsigned int) RTC.CNT);
    uint8_t board[height][byte_width];
    for (uint8_t y=0; y<height;y++){
        for (uint8_t x=0; x<byte_width;x++){
            board[y][x] = rand();
        };
    };
    
    while (1){
        display_board(board);
        update_board(board);
        //_delay_ms(50);
        if (button_pressed) {
            return;
        }
    }
}
