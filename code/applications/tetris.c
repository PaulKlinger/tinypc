#include "tetris.h"
#include "../mcc_generated_files/config/clock_config.h"
#include <util/delay.h>
#include "../lcd.h"
#include "../utilities.h"


// board dimensions in blocks
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

// height/width of blocks in pixel
#define BLOCK_DIM 3

#define X_OFFSET ((128 - BOARD_WIDTH * BLOCK_DIM) / 2)
#define Y_OFFSET 3



#define NUM_PIECES  7
enum PieceType {
    P_I, P_J, P_L, P_O, P_S, P_T, P_Z
};

// block shapes defined on 2x4 grid, i.e. the bits are arranged like
// 0 1 2 3
// 4 5 6 7
// 
// E.g. 0b100001110 gives
// X . . .
// X X X .
static const uint8_t block_shapes[] = {
    0b11110000, 0b10001110, 0b00101110, 0b01100110, 0b01101100, 0b01001110, 0b11000110};


struct TetrisGamestate {
    uint16_t points;
    enum PieceType current_piece_type;
    uint8_t current_piece_x;
    uint8_t current_piece_y;
    Direction current_piece_orientation;
    enum PieceType next_piece_type;
    Direction next_piece_orientation;
    // shows whether to draw in black or white and whether a collision
    // occurred. Somewhat weird to put this here but this makes
    // apply_to_piece_blocks easier.
    uint8_t flag;
    // true if piece is falling, false if it hit something and stopped
    bool piece_moving; 
};

bool check_block(uint8_t x, uint8_t y) {
    return lcd_check_buffer(X_OFFSET + x * BLOCK_DIM,
                            Y_OFFSET + y * BLOCK_DIM);
}

void draw_block(uint8_t x, uint8_t y, struct TetrisGamestate *state) {
    if (x > BOARD_WIDTH || y > BOARD_HEIGHT) return;
    lcd_fillRect(X_OFFSET + x * BLOCK_DIM, Y_OFFSET + y * BLOCK_DIM,
                 X_OFFSET + (x + 1) * BLOCK_DIM - 1, Y_OFFSET + (y + 1) * BLOCK_DIM - 1,
            state->flag);
    
}

void collide_block(uint8_t x, uint8_t y, struct TetrisGamestate *state) {
    // we need to check for side wall collision even if block is outside the play area
    // otherwise it can be moved into an illegal position before it is fully inside
    if (x >= BOARD_WIDTH ) { // don't need to check other direction due to wraparound
        state->flag = 1;
        return;
    }
    if ((y >= BOARD_HEIGHT && y < 255 - 4) // don't show collision for blocks that enter the play area 
            || check_block(x, y)) {
        state->flag = 1;
    }
}

void collide_block_top(uint8_t x, uint8_t y, struct TetrisGamestate *state) {
    if (y > 255 - 4) {
        state->flag = 1;
    }
}

void apply_to_piece_blocks(void (*f)(uint8_t, uint8_t, struct TetrisGamestate*),
        struct TetrisGamestate *state) {
    uint8_t x = state->current_piece_x;
    uint8_t y = state->current_piece_y;
    for (uint8_t p_x=0; p_x < 4; p_x++){
        for (uint8_t p_y=0; p_y < 2; p_y++) {
            if (block_shapes[state->current_piece_type] & (1 << (p_x + 4 * p_y))) {
                switch (state->current_piece_orientation){
                    case UP:
                        (*f)(x + p_x, y + 1 - p_y, state);
                        break;
                    case DOWN:
                        (*f)(x + 3 - p_x, y + p_y, state);
                        break;
                    case LEFT:
                        (*f)(x + p_y + 1, y - 1 + p_x, state);
                        break;
                    case RIGHT:
                        (*f)(x + 2 - p_y, y - 1 + 3 - p_x, state);
                        break;
                }
            }
        }
    }
}

void new_piece(struct TetrisGamestate *state) {
    state->current_piece_type = state->next_piece_type;
    state->current_piece_orientation = state->next_piece_orientation;
    state->next_piece_type = rand() % NUM_PIECES;
    state->next_piece_orientation = rand() % 4;
    state->current_piece_y = 254;
    state->current_piece_x = BOARD_WIDTH/2 - 1;
    state->piece_moving = true;
}

void rotate_piece(struct TetrisGamestate *state) {
    // draw black over previous piece location
    state->flag = 0;
    apply_to_piece_blocks(&draw_block, state);
    
    state->current_piece_orientation = (state->current_piece_orientation + 1) % 4;
    uint8_t x = state->current_piece_x;
    // if the piece doesn't fit in the current location with the new orientation
    // we shift it around up to 2 spaces and test again
    for (uint8_t offset=0; offset <=2; offset++) {
        for (int8_t offset_direction=-1; offset_direction <= 1; offset_direction += 2) {
            state->current_piece_x = x + offset_direction * offset;
            state->flag = 0;
            apply_to_piece_blocks(&collide_block, state);
            if (state->flag == 0) {
                // if there is no collision draw the block in the new
                // orientation and return
                state->flag = 1;
                apply_to_piece_blocks(&draw_block, state);
                return;
            }
        }
    }
    // if we can't find a place for it we rotate it back and return
    state->current_piece_x = x;
    state->current_piece_orientation = (state->current_piece_orientation + 3) % 4;
}

void move_piece_down_and_collide(struct TetrisGamestate *state) {
    // draw black over previous piece location
    state->flag = 0;
    apply_to_piece_blocks(&draw_block, state);
    // move down and check for collision
    state->current_piece_y++;
    apply_to_piece_blocks(&collide_block, state);
    if (state->flag) {
        // collision! draw piece in previous location and set piece_moving to false
        state->current_piece_y--;
        apply_to_piece_blocks(&draw_block, state); // (state->flag is 1)
        state->piece_moving = false;
    }
    // otherwise draw it in the new location
    state->flag = 1;
    apply_to_piece_blocks(&draw_block, state);
}

void move_piece_x(struct TetrisGamestate *state, int8_t dx) {
    state->flag = 0;
    apply_to_piece_blocks(&draw_block, state);
    state->current_piece_x += dx;
    apply_to_piece_blocks(&collide_block, state);
    if (state->flag) {
        // if there is a collision we move the piece back in the original location
        state->current_piece_x -= dx;
    }
    state->flag = 1;
    apply_to_piece_blocks(&draw_block, state);
}

void check_and_handle_full_lines(struct TetrisGamestate *state) {
    uint8_t n_full_lines = 0;
    for (uint8_t y=0; y < BOARD_HEIGHT; y++) {
        bool full_line = true;
        for (uint8_t x=0; x < BOARD_WIDTH; x++) {
            if (!lcd_check_buffer(X_OFFSET + x * BLOCK_DIM,
                                  Y_OFFSET + y * BLOCK_DIM)) {
                full_line = false;
            }
        }
        if (full_line) {
            n_full_lines++;
            if (y > 0) {
                // move blocks down into the cleared line
                for (uint8_t dy = 1; dy <= y; dy++) {
                    for (uint8_t x=0; x < BOARD_WIDTH; x++) {
                        // copy block status down
                        state->flag = check_block(x, y-dy);
                        draw_block(x, y-dy+1, state);
                        // delete upper block
                        state->flag = 0;
                        draw_block(x, y-dy, state);
                    }
                }
                // the cleared line is now filled with the blocks from the
                // next one above, so we need to check it again
                y--;
            }
        }
    }
    switch (n_full_lines) {
        case 1:
            state->points += 4;
            break;
        case 2:
            state->points += 10;
            break;
        case 3:
            state->points += 30;
            break;
        case 4:
            state->points += 120;
            break;
    }
}

void draw_game_area() {
    lcd_drawLine(X_OFFSET - 1, Y_OFFSET - 1,
        X_OFFSET  + BLOCK_DIM * BOARD_WIDTH, Y_OFFSET - 1, 1);
    lcd_drawLine(X_OFFSET - 1, Y_OFFSET - 1, X_OFFSET - 1, Y_OFFSET + BLOCK_DIM * BOARD_HEIGHT, 1);
    lcd_drawLine(X_OFFSET  + BLOCK_DIM * BOARD_WIDTH, Y_OFFSET - 1,
        X_OFFSET  + BLOCK_DIM * BOARD_WIDTH, BLOCK_DIM * BOARD_HEIGHT + Y_OFFSET, 1);
    lcd_drawLine(X_OFFSET - 1, Y_OFFSET + BLOCK_DIM * BOARD_HEIGHT,
        X_OFFSET  + BLOCK_DIM * BOARD_WIDTH, Y_OFFSET + BLOCK_DIM * BOARD_HEIGHT, 1);
}

void draw_score(uint16_t score) {
    lcd_fillRect(0, 8, 40, 16, 0);
    char points_str[5];
    ltoa(score, points_str, 10);
    lcd_gotoxy(0,2);
    lcd_puts(points_str);
}


void draw_block_no_bounds(uint8_t x, uint8_t y, struct TetrisGamestate *state) {
    lcd_fillRect(X_OFFSET + x * BLOCK_DIM, Y_OFFSET + y * BLOCK_DIM,
                 X_OFFSET + (x + 1) * BLOCK_DIM - 1, Y_OFFSET + (y + 1) * BLOCK_DIM - 1,
            state->flag);
    
}

void draw_next_piece(struct TetrisGamestate *state) {
    lcd_fillRect(X_OFFSET + BOARD_WIDTH * BLOCK_DIM +  BLOCK_DIM, Y_OFFSET + 2* BLOCK_DIM,
            X_OFFSET + BOARD_WIDTH * BLOCK_DIM + 10 * BLOCK_DIM, 8 + 7 * BLOCK_DIM, 0);
    struct TetrisGamestate tempstate;
    tempstate.current_piece_orientation = state->next_piece_orientation;
    tempstate.current_piece_type = state->next_piece_type;
    tempstate.current_piece_x = BOARD_WIDTH + 4;
    tempstate.current_piece_y = 4;
    tempstate.flag = 1;
    apply_to_piece_blocks(&draw_block_no_bounds, &tempstate);
}

void run_tetris(void) {
    struct TetrisGamestate state;
    state.points = 0;
    
    lcd_clrscr();
    draw_game_area();
    while(button_pressed);
    
    uint8_t counter = 0;
    uint8_t joystick_pressed_counter = 0;
    
    // run new_piece twice to fill next_piece
    new_piece(&state);
    while (true) {
        draw_score(state.points);
        set_led_from_points(state.points, 500);
        new_piece(&state);
        draw_next_piece(&state);
        while (state.piece_moving) {
            if (joystick_pressed) {
                joystick_pressed_counter++;
                switch (last_joystick_direction) {
                    case UP:
                        if (joystick_pressed_counter == 1) {
                            rotate_piece(&state);
                        }
                        break;
                    case LEFT:
                        if (joystick_pressed_counter % 3 == 1) {
                            move_piece_x(&state, -1);
                        }
                        break;
                    case RIGHT:
                        if (joystick_pressed_counter % 3 == 1 ) {
                            move_piece_x(&state, 1);
                        }
                        break;
                    case DOWN:
                        move_piece_down_and_collide(&state);
                        break;
                }
            } else {
                joystick_pressed_counter = 0;
            }
            lcd_display();
            counter = (counter + 1) % (30 - 27 * MIN(state.points, 500) / 500);
            if (counter == 0 && state.piece_moving) {
                move_piece_down_and_collide(&state);
            }
            _delay_ms(10);
        }
        check_and_handle_full_lines(&state);
        state.flag = 0;
        apply_to_piece_blocks(&collide_block_top, &state);
        if (state.flag) {
            show_game_over_screen(state.points);
            return;
        }
    }
}
