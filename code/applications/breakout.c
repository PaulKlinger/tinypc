#include "breakout.h"
#include "../mcc_generated_files/config/clock_config.h"
#include <util/delay.h>
#include <string.h>
#include "../lcd.h"
#include "../utilities.h"

#define num_blocks 18
#define ball_speed 1 // speed of ball / frame
#define ball_radius 2.5
#define ball_int_radius 2

static bool get_block_status(uint8_t block_id, uint8_t *block_status) {
    return block_status[block_id / 8] & (1 << (block_id % 8));
}

static bool destroy_block(uint8_t block_id, uint8_t *block_status) {
    return block_status[block_id / 8] &= ~(1 << (block_id % 8));
}

struct BlockCoords{
    uint8_t x1, y1, x2, y2;
};

static struct BlockCoords calc_block_coords(uint8_t block_id) {
    struct BlockCoords ret;
    ret.x1 = (block_id % (DISPLAY_WIDTH / 21)) * 21 + 1;
    ret.y1 = block_id / (DISPLAY_WIDTH / 21) * 5;
    ret.x2 = ret.x1 + 19;
    ret.y2 = ret.y1 + 3;
    return ret;
}

static void draw_block(uint8_t block_id) {
    struct BlockCoords coords = calc_block_coords(block_id);
    lcd_fillRect(coords.x1, coords.y1, coords.x2, coords.y2, 1);
}

static void draw_blocks(uint8_t *block_status) {
    for (uint8_t i=0; i<num_blocks; i++) {
        if (get_block_status(i, block_status)) {
            draw_block(i);
        }
    }
}

static void display_block(uint8_t block_id) {
    struct BlockCoords coords = calc_block_coords(block_id);
    lcd_display_block(coords.x1, coords.y1/8, 20);
    if (coords.y2/8 != coords.y1/8) {
        lcd_display_block(coords.x1, coords.y2/8, 20);
    }
};

typedef struct {
    float x, y, vx, vy;
    // for partial display updates
    // tline means topmost display line intersecting the ball
    uint8_t prev_x, prev_tline; 
} Ball;

static void display_ball(Ball *ball) {
    // updating only one line when the ball lies completely on one 
    // would be faster, but unless updates are changed to use timer interrupts
    // need to do same every frame to keep frametimes consistent
    
    uint8_t current_tline = (round(ball->y) - ball_int_radius) / 8;
    
    lcd_display_block(round(ball->x) - ball_int_radius, current_tline, 5);
    lcd_display_block(round(ball->x) - ball_int_radius, current_tline + 1, 5);
    lcd_display_block(ball->prev_x - ball_int_radius, ball->prev_tline, 5);
    lcd_display_block(ball->prev_x - ball_int_radius, ball->prev_tline + 1, 5);
}

static void draw_ball(Ball *ball) {
    //lcd_fillCircle(round(ball->x), round(ball->y), 2, 1);
    uint8_t x = round(ball->x);
    uint8_t y = round(ball->y);
    for (int8_t dx=-ball_int_radius; dx <= ball_int_radius; dx++) {
        for (int8_t dy=-ball_int_radius; dy <= ball_int_radius; dy++){
            if (!(abs(dy) == ball_int_radius && abs(dx) == ball_int_radius)){
                lcd_drawPixel(x+dx, y+dy, 1);
            }
        }
    }
}

static void normalize_ball_v(Ball *ball) {
    float current_speed = sqrt(ball->vx * ball->vx + ball->vy * ball->vy);
    ball->vx *= ball_speed / current_speed;
    ball->vy *= ball_speed / current_speed;
}

static void wall_collision(Ball *ball) {
    if (ball->x < ball_radius) {
        ball->y = ball->y - (ball->x - ball_radius) / ball->vx * ball->vy;
        ball->x = ball_radius;
        ball->vx *= -1;
    } else if (ball->x > DISPLAY_WIDTH - ball_radius) {
        ball->y = ball->y - (ball->x -DISPLAY_WIDTH + ball_radius) / ball->vx * ball->vy;
        ball->x = DISPLAY_WIDTH - ball_radius;
        ball->vx *= -1;
    } else if (ball->y > DISPLAY_HEIGHT - ball_radius) {
        ball->x = ball->x - (ball->y - DISPLAY_HEIGHT + ball_radius) / ball->vy * ball->vx;
        ball->y = DISPLAY_HEIGHT - ball_radius;
        ball->vy *= -1;
    } else if (ball->y < ball_radius) {
        ball->x = ball->x - (ball->y - ball_radius) / ball->vy * ball->vx;
        ball->y = ball_radius;
        ball->vy *= -1;
    }
}

static inline bool ball_intersects_block(Ball *ball, struct BlockCoords coords) {
    return ball->x + ball_radius > coords.x1
            && ball->x - ball_radius < coords.x2 
            && ball->y + ball_radius > coords.y1
            && ball->y - ball_radius < coords.y2;
}

static void block_collision(Ball *ball, uint8_t *block_status) {
    struct BlockCoords coords;
    for (uint8_t i=0; i < num_blocks; i++) {
        if (get_block_status(i, block_status)) {
            coords = calc_block_coords(i);
            if (ball_intersects_block(ball, coords)) {
                destroy_block(i, block_status);
                
                // move backwards slowly until we don't intersect anymore
                do {
                    ball->x -= ball->vx / 10;
                    ball->y -= ball->vy / 10;
                } while (ball_intersects_block(ball, coords));
                
                // if we are right or left the block we hit the sides
                if (ball->x - ball_radius > coords.x2 
                        || ball->x + ball_radius < coords.x1) {
                    ball->vx *= -1;
                }
                // if we are below or above the block we hit the bottom/top
                // both happen if we exactly hit the corner
                if (ball->y - ball_radius > coords.y2 
                        || ball->y + ball_radius < coords.y1) {
                    ball->vy *= -1;
                }
            }
        }
    }
}

static void handle_collisions(Ball *ball, uint8_t *block_status) {
    wall_collision(ball);
    block_collision(ball, block_status);
}

static void move_ball(Ball *ball) {
    ball->prev_x = round(ball->x);
    ball->prev_tline = (round(ball->y) - ball_int_radius) / 8;
    ball->x += ball->vx;
    ball->y += ball->vy;
}

void run_breakout() {
    uint8_t block_status[num_blocks / 8 + (num_blocks % 8 ? 1 : 0)];
    uint8_t prev_block_status[num_blocks / 8 + (num_blocks % 8 ? 1 : 0)];
    memset(&block_status, 0xFF, num_blocks / 8 + (num_blocks % 8 ? 1 : 0));
    memset(&prev_block_status, 0x00, num_blocks / 8 + (num_blocks % 8 ? 1 : 0));
    
    Ball ball = {.x = DISPLAY_WIDTH/2, .y = (DISPLAY_HEIGHT - 4), .vx = 3, .vy = -10};
    normalize_ball_v(&ball);
    
    lcd_clear_buffer();
    draw_blocks(block_status);
    draw_ball(&ball);
    lcd_display();
    wait_for_button();
    while (button_pressed);
    lcd_clear_buffer();
    lcd_display();
    while (!button_pressed) {
        lcd_clear_buffer();
        move_ball(&ball);
        handle_collisions(&ball, block_status);
        draw_blocks(block_status);
        draw_ball(&ball);
        display_ball(&ball);
        for (uint8_t i=0; i < num_blocks; i++) {
            if (get_block_status(i, block_status) != 
                    get_block_status(i, prev_block_status)) {
                display_block(i);
            }
        }
        _delay_ms(16);
        memcpy(prev_block_status, block_status, sizeof(prev_block_status));
    }
}
