#include "mines.h"
#include "../mcc_generated_files/config/clock_config.h"
#include "../lcd.h"
#include "../utilities.h"
#include <util/delay.h>


#define GRID_X 16
#define GRID_Y 10
#define X_OFFSET 20
#define Y_OFFSET 2

#define CELL_SIZE 6

#define NMINES 25

enum MinesGamePhase {
    m_FIRSTMOVE,
    m_GAME,
    m_END
};

struct MinesGameState {
    uint8_t cursor_x, cursor_y;
    BitMatrix mines;
    BitMatrix revealed;
    uint16_t n_revealed;
    BitMatrix flagged;
    uint16_t n_flagged;
    enum MinesGamePhase game_phase;
};

/* 3x3 digit font
 bit order (from msb to lsb) for first character
 0 1 2 
 3 4 5
 6 7 8

 (8 is the zeroth bit of the next byte)
 here the zero is replaced with an empty character to de-clutter the board
 the full version is commented out below
 */
static const uint8_t digit_font[] = {0b00000000,0b01100101, 0b11110010, 0b01111101,
0b11111011, 0b11001011, 0b01011010, 0b01111111, 0b11001001, 0b1111111,
0b11111110, 0b01000000};

//static const uint8_t digit_font[] = {0b11110111,0b11100101, 0b11110010, 0b01111101,
//0b11111011, 0b11001011, 0b01011010, 0b01111111, 0b11001001, 0b1111111,
//0b11111110, 0b01000000};


// these functions give the pixel coordinates of the top left corner the cell
// (cell_x, cell_y)
static inline uint8_t x_cell(uint8_t cell_x) {
    return X_OFFSET + cell_x * CELL_SIZE;
}
static inline uint8_t y_cell(uint8_t cell_y) {
    return Y_OFFSET + cell_y * CELL_SIZE;
}

static void draw_digit(uint8_t digit, uint8_t cell_x, uint8_t cell_y) {
    uint8_t x = 2 + x_cell(cell_x);
    uint8_t y = 2 + y_cell(cell_y);
    
    for (uint8_t dy=0;dy<3;dy++) {
        for (uint8_t dx=0; dx<3; dx++) {
            uint8_t bit_index = digit * 9 + dy * 3 + dx;
            lcd_drawPixel(x+dx, y+dy, !!(digit_font[bit_index / 8] & (1 << (7 - bit_index % 8))));
        }
    }
}

static void draw_hidden_cell(uint8_t cell_x, uint8_t cell_y) {
    lcd_drawPixel(3 + x_cell(cell_x),
            3 + y_cell(cell_y), 1);
}

static void draw_mine(uint8_t cell_x, uint8_t cell_y) {
    // draw a cross for mines and flags
    for (uint8_t i=0; i<5; i++) {
        lcd_drawPixel(x_cell(cell_x) + 1 + i,
                y_cell(cell_y) + 1 + i, 1);
        lcd_drawPixel(x_cell(cell_x) + 1 + i,
                y_cell(cell_y) + 5 - i, 1);
    }
}

static void draw_grid() {
    for (uint8_t cell_x=0; cell_x<=GRID_X; cell_x++) {
        lcd_drawLine(x_cell(cell_x), Y_OFFSET, x_cell(cell_x), y_cell(GRID_Y), 1);
    }
    for (uint8_t cell_y=0; cell_y <= GRID_Y; cell_y++){
        lcd_drawLine(X_OFFSET, y_cell(cell_y), x_cell(GRID_X), y_cell(cell_y), 1);
    }
}

static void draw_cursor(struct MinesGameState *state) {
    lcd_drawRect(x_cell(state->cursor_x) + 1,
            y_cell(state->cursor_y) + 1,
            x_cell(state->cursor_x) + 5,
            y_cell(state->cursor_y) + 5, 1);
}


// noinline to save memory
__attribute__ ((noinline)) static uint8_t mines_count(struct MinesGameState *state, uint8_t cell_x, uint8_t cell_y) {
    uint8_t nmines = 0;
    for (int8_t dy=-1; dy <= 1; dy++) {
        for (int8_t dx=-1; dx <= 1; dx++){
            if (cell_x + dx >= GRID_X || cell_x + dx < 0 || cell_y + dy >= GRID_Y || cell_y + dy < 0) continue;
            nmines += bitmatrix_get(state->mines,
                    cell_x+dx, cell_y + dy);
        }
    }
    return nmines;
}

static void init_mines(struct MinesGameState *state) {
    for (uint8_t i=0; i<NMINES; i++) {
        while (true) {
            uint8_t cell_x = randint(0, GRID_X - 1);
            uint8_t cell_y = randint(0, GRID_Y - 1);
            
            // Do not set the same mine twice and do not set a mine at the 
            // cursor location.
            if (!bitmatrix_get(state->mines, cell_x, cell_y) 
                    && (abs((int16_t) cell_x - state->cursor_x) > 1 || abs((int16_t) cell_y - state->cursor_y) > 1)) {
                bitmatrix_set(state->mines, cell_x, cell_y);
                break;
            }
        }
    }
}

static void draw_remaining_mine_count(struct MinesGameState *state) {
    char mine_count_str[4];
    itoa(NMINES - state->n_flagged, mine_count_str, 10);
    lcd_gotoxy(0,1);
    lcd_puts(mine_count_str);
}

static void show_game_state(struct MinesGameState *state) {
    lcd_clear_buffer();
    draw_grid();
    for (uint8_t cell_y=0; cell_y < GRID_Y; cell_y++){
        for (uint8_t cell_x=0; cell_x < GRID_X; cell_x++){
            if (bitmatrix_get(state->flagged, cell_x, cell_y)
                    || (state->game_phase == m_END
                        && bitmatrix_get(state->mines, cell_x, cell_y))) {   
                draw_mine(cell_x, cell_y);
            } else if (state->game_phase != m_END && !bitmatrix_get(state->revealed, cell_x, cell_y)) {
                draw_hidden_cell(cell_x, cell_y);
            } else {
                draw_digit(mines_count(state, cell_x, cell_y), cell_x, cell_y);
            }
        }
    }
    draw_cursor(state);
    draw_remaining_mine_count(state);
    lcd_display();
}

__attribute__ ((optimize("s")))
static void reveal_cell(struct MinesGameState *state, uint8_t cell0_x, uint8_t cell0_y) {
    uint8_t to_reveal_data[(GRID_X / 8) * GRID_Y] = {0};
    BitMatrix to_reveal = {.byte_width=GRID_X / 8, .data=to_reveal_data };
    
    bitmatrix_set(to_reveal, cell0_x, cell0_y);
    uint16_t n_to_reveal = 1;
    
    // flood fill to reveal adjacent cells of zero mine-neighbor cells
    // need to do this non-recursively as stack overflows otherwise
    // (Would be a lot faster if it used a list of locations instead of
    //  to_reveal BitMatrix, but not sure I could manage that with less code.)
    while (n_to_reveal > 0) {
        for (uint8_t cell_y=0; cell_y < GRID_Y; cell_y++){
            for (uint8_t cell_x=0; cell_x < GRID_X; cell_x++) {
                if (bitmatrix_get(to_reveal, cell_x, cell_y)) {
                    bitmatrix_unset(to_reveal, cell_x, cell_y);
                    n_to_reveal--;
                    bitmatrix_set(state->revealed, cell_x, cell_y);
                    state->n_revealed++;
                    if (mines_count(state, cell_x, cell_y) == 0) {
                        for (int8_t dy=-1; dy <= 1; dy++) {
                            for (int8_t dx=-1; dx <= 1; dx++){
                                if (cell_x + dx >= GRID_X || cell_x + dx < 0 || cell_y + dy >= GRID_Y || cell_y + dy < 0) continue;
                                if (dx==0 && dy==0) continue;
                                if (!bitmatrix_get(state->mines, cell_x + dx, cell_y + dy)
                                        && !bitmatrix_get(to_reveal, cell_x + dx, cell_y + dy)
                                        && !bitmatrix_get(state->revealed, cell_x + dx, cell_y + dy)) {
                                    bitmatrix_set(to_reveal, cell_x + dx, cell_y + dy);
                                    n_to_reveal++;
                                }                
                            }
                        }
                    }
                }
            }
        }
        
        
    }
}

static void boom(struct MinesGameState *state) {
    for (uint8_t i=0; i<30; i++) {
        lcd_fillCircleSimple(x_cell(state->cursor_x) + CELL_SIZE / 2, y_cell(state->cursor_y) + CELL_SIZE / 2, i, 1);
        lcd_display();
    }
}

void run_mines(void) {
    uint8_t mines_data[(GRID_X / 8) * GRID_Y] = {0};
    BitMatrix mines = {.byte_width = GRID_X / 8, .data = mines_data};
    uint8_t revealed_data[(GRID_X / 8) * GRID_Y] = {0};
    BitMatrix revealed = {.byte_width = GRID_X / 8, .data = revealed_data};
    uint8_t flagged_data[(GRID_X / 8) * GRID_Y] = {0};
    BitMatrix flagged = {.byte_width = GRID_X / 8, .data = flagged_data};
    
    struct MinesGameState state = {
        .mines=mines, .revealed=revealed, .flagged=flagged,
        .game_phase=m_FIRSTMOVE};
    
    while (button_pressed);
    
    while (state.game_phase != m_END) {        
        if (joystick_pressed) {
            while (joystick_pressed);
            switch (last_joystick_direction) {
                case UP:
                    if (state.cursor_y > 0) state.cursor_y--;
                    break;
                case DOWN:
                    if (state.cursor_y < GRID_Y - 1) state.cursor_y++;
                    break;
                case LEFT:
                    if (state.cursor_x > 0) state.cursor_x--;
                    break;
                case RIGHT:
                    if (state.cursor_x < GRID_X - 1) state.cursor_x++;
                    break;
            }
        } else if (button_pressed) {
            uint16_t count = 0;
            while (button_pressed) {
                count++;
                _delay_ms(10);
            };
            if (count > 50) {
                if (bitmatrix_get(state.flagged, state.cursor_x, state.cursor_y)){
                    bitmatrix_unset(state.flagged, state.cursor_x, state.cursor_y);
                    state.n_flagged--;
                } else {
                    bitmatrix_set(state.flagged, state.cursor_x, state.cursor_y);
                    state.n_flagged++;
                }
            } else if (!bitmatrix_get(state.flagged, state.cursor_x, state.cursor_y)) {
                // Don't reveal flagged cells, this is most likely a misclick.
                if (bitmatrix_get(state.mines, state.cursor_x, state.cursor_y)) {
                    boom(&state);
                    state.game_phase = m_END;
                } else {
                    if (state.game_phase == m_FIRSTMOVE) {
                        init_mines(&state);
                        state.game_phase = m_GAME;
                    }
                    reveal_cell(&state, state.cursor_x, state.cursor_y);
                    if (state.n_revealed == GRID_X * GRID_Y - NMINES) {
                        lcd_clear_buffer();
                        lcd_gotoxy(3, 3);
                        lcd_puts("You won!");
                        lcd_display();
                        while(!button_pressed);
                        state.game_phase = m_END;
                    }
                }
            }
            
        }
        
        show_game_state(&state);
        set_led_from_points(state.n_flagged, NMINES);
    }
    
    // keep showing board until button press so player can see their mistake
    wait_for_button();
}
