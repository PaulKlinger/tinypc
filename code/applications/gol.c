#include "../mcc_generated_files/config/clock_config.h"
#include <avr/io.h>
#include <util/delay.h>

#include "../lcd.h"
#include "../utilities.h"

#define height 32
#define width 64
#define byte_width 8

static bool get_cell(uint8_t board[height][byte_width], uint8_t x, uint8_t y) {
    return board[y][x / 8] & (1 << (x % 8));
}

static bool set_cell(uint8_t board[height][byte_width], uint8_t x, uint8_t y) {
    return board[y][x / 8] |= (1 << (x % 8));
}

static bool unset_cell(uint8_t board[height][byte_width], uint8_t x, uint8_t y) {
    return board[y][x / 8] &= ~(1 << (x % 8));
}

static bool get_cell_from_buffer(uint8_t x, uint8_t y) {
    return lcd_check_buffer(x*2, y*2);
}

static void draw_cell(uint8_t x, uint8_t y) {
    lcd_fillRect(x*2, y*2, x*2+1, y*2+1, 1);
}

static void display_board(uint8_t board[height][byte_width]) {
    lcd_clear_buffer();
    uint16_t live_cells = 0;
    for (uint8_t y=0; y<height;y++){
        for (uint8_t x=0; x<width;x++){
            if (get_cell(board, x, y)){
                draw_cell(x,y);
                live_cells++;
            }
        };
    };
    lcd_display();
    set_led_from_points(live_cells, height * width / 2);
}

static void update_board(uint8_t board[height][byte_width]) {
    uint8_t neighbors;
    
    // TODO: This section is actually performance critical
    // (updates become seriously slow if modulo isn't inlined).
    // Should optimize at some point to directly use board without
    // get_cell_from_buffer.
    
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
    uint8_t board[height][byte_width];
    for (uint8_t y=0; y<height;y++){
        for (uint8_t x=0; x<byte_width;x++){
            board[y][x] = rand();
        };
    };
    bool wait_for_release = button_pressed;
    while (1){
        if (!button_pressed){
            wait_for_release = false;
        }
        display_board(board);
        update_board(board);
        if (button_pressed && ! wait_for_release) {
            return;
        }
    }
}
