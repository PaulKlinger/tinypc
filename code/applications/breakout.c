#include "breakout.h"
#include "../mcc_generated_files/config/clock_config.h"
#include <util/delay.h>
#include <string.h>
#include "../lcd.h"
#include "../utilities.h"
#include <stdfix.h>

#define num_blocks 24
#define initial_ball_speed 1.5K // speed of ball / frame
#define initial_ball_vx_unscaled 3K 
#define initial_ball_vy_unscaled -10K
#define norm sqrt(initial_ball_vx_unscaled * initial_ball_vx_unscaled + initial_ball_vy_unscaled * initial_ball_vy_unscaled)
#define initial_ball_vx (initial_ball_speed * (initial_ball_vx_unscaled / norm))
#define initial_ball_vy (initial_ball_speed * (initial_ball_vy_unscaled / norm))
#define ball_speed_increase_factor 1.025K // factor speed increases by each block
#define ball_radius 2.5K
#define ball_int_radius 2
#define paddle_width 25
#define paddle_height 3
#define paddle_speed 3.5K


typedef struct {
    AccVec pos, v;
    // for partial display updates
    // tline means topmost display line intersecting the ball
    uint8_t prev_x, prev_tline; 
} Ball;

typedef struct {
    Ball ball;
    accum paddle_x;
    uint8_t block_status[num_blocks / 8 + (num_blocks % 8 ? 1 : 0)];
    uint8_t prev_block_status[num_blocks / 8 + (num_blocks % 8 ? 1 : 0)];
} BreakoutGamestate;

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

static void display_ball(Ball *ball) {
    // updating only one line when the ball lies completely on one 
    // would be faster, but unless updates are changed to use timer interrupts
    // need to do same every frame to keep frametimes consistent
    
    uint8_t current_tline = (roundacc0(ball->pos.y) - ball_int_radius) / 8;
    
    lcd_display_block(roundacc0(ball->pos.x) - ball_int_radius, current_tline, 5);
    lcd_display_block(roundacc0(ball->pos.x) - ball_int_radius, current_tline + 1, 5);
    lcd_display_block(ball->prev_x - ball_int_radius, ball->prev_tline, 5);
    lcd_display_block(ball->prev_x - ball_int_radius, ball->prev_tline + 1, 5);
}

static void draw_ball(Ball *ball) {
    //lcd_fillCircle(round(ball->pos.x), round(ball->pos.y), 2, 1);
    uint8_t x = roundacc0(ball->pos.x);
    uint8_t y = roundacc0(ball->pos.y);
    for (int8_t dx=-ball_int_radius; dx <= ball_int_radius; dx++) {
        for (int8_t dy=-ball_int_radius; dy <= ball_int_radius; dy++){
            if (!(abs(dy) == ball_int_radius && abs(dx) == ball_int_radius)){
                lcd_drawPixel(x+dx, y+dy, 1);
            }
        }
    }
}

static void wall_collision(Ball *ball) {
    if (ball->pos.x < ball_radius) {
        ball->pos.y = ball->pos.y - (ball->pos.x - ball_radius) / ball->v.x * ball->v.y;
        ball->pos.x = ball_radius;
        ball->v.x *= -1;
    } else if (ball->pos.x > DISPLAY_WIDTH - ball_radius) {
        ball->pos.y = ball->pos.y - (ball->pos.x - DISPLAY_WIDTH + ball_radius) / ball->v.x * ball->v.y;
        ball->pos.x = DISPLAY_WIDTH - ball_radius;
        ball->v.x *= -1;
    } else if (ball->pos.y < ball_radius) {
        ball->pos.x = ball->pos.x - (ball->pos.y - ball_radius) / ball->v.y * ball->v.x;
        ball->pos.y = ball_radius;
        ball->v.y *= -1;
    } 
}

static bool bottom_collision(Ball *ball) {
    if (ball->pos.y > DISPLAY_HEIGHT - ball_radius) {
        return true;
    }
    return false;
}

static bool ball_intersects_block(Ball *ball, struct BlockCoords coords) {
    return ball->pos.x + ball_radius > coords.x1
            && ball->pos.x - ball_radius < coords.x2 
            && ball->pos.y + ball_radius > coords.y1
            && ball->pos.y - ball_radius < coords.y2;
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
                    ball->pos.x -= ball->v.x / 10;
                    ball->pos.y -= ball->v.y / 10;
                } while (ball_intersects_block(ball, coords));
                
                // if we are right or left the block we hit the sides
                if (ball->pos.x - ball_radius > coords.x2 
                        || ball->pos.x + ball_radius < coords.x1) {
                    ball->v.x *= -1;
                }
                // if we are below or above the block we hit the bottom/top
                // both happen if we exactly hit the corner
                if (ball->pos.y - ball_radius > coords.y2 
                        || ball->pos.y + ball_radius < coords.y1) {
                    ball->v.y *= -1;
                }
            }
        }
    }
}

static struct BlockCoords calc_paddle_coords(accum paddle_x) {
    struct BlockCoords ret;
    ret.x1 = roundacc0(paddle_x) - (paddle_width - 1) / 2;
    ret.y1 = DISPLAY_HEIGHT - 1 - paddle_height;
    ret.x2 = roundacc0(paddle_x) + (paddle_width - 1) / 2;
    ret.y2 = DISPLAY_HEIGHT - 1;
    return ret;
}

static void paddle_collision(Ball *ball, accum paddle_x) {
    struct BlockCoords paddle_coords = calc_paddle_coords(paddle_x);
    if (ball_intersects_block(ball, paddle_coords)) {
        do {
            ball->pos.x -= ball->v.x / 10;
            ball->pos.y -= ball->v.y / 10;
        } while (ball_intersects_block(ball, paddle_coords));
        ball->v.y *= -1;
        // The direction of the outgoing ball depends on the impact point
        // on the paddle. This makes the game more interesting and prevents
        // getting stuck in a loop.
        rotate_vec(&(ball->v), (ball->pos.x - paddle_x));
    }
}

static void handle_collisions(Ball *ball, uint8_t *block_status, accum paddle_x) {
    wall_collision(ball);
    block_collision(ball, block_status);
    paddle_collision(ball, paddle_x);
}

static void move_ball(Ball *ball) {
    ball->prev_x = roundacc0(ball->pos.x);
    ball->prev_tline = (roundacc0(ball->pos.y) - ball_int_radius) / 8;
    ball->pos.x += ball->v.x;
    ball->pos.y += ball->v.y;
}

static void draw_paddle(accum paddle_x){
    uint8_t xmin = roundacc0(paddle_x) - (paddle_width - 1) / 2;
    lcd_fillRect(xmin, DISPLAY_HEIGHT - 1 - paddle_height,
                 xmin + paddle_width, DISPLAY_HEIGHT - 1, 1);
}
static void display_paddle(accum paddle_x) {
    uint8_t xmin = roundacc0(paddle_x) - (paddle_width - 1) / 2;
    lcd_display_block(xmin < ceilacc8(paddle_speed) ? 0 : xmin - ceilacc8(paddle_speed),
                      DISPLAY_HEIGHT / 8 - 1, paddle_width + 1 + 2 * ceilacc8(paddle_speed));
}

static BreakoutGamestate init_gamestate() {
    Ball ball = {.pos = {DISPLAY_WIDTH/2, (DISPLAY_HEIGHT - 6)},
                 .v = {initial_ball_vx, initial_ball_vy}};
    BreakoutGamestate state;
    state.ball = ball;
    state.paddle_x = DISPLAY_WIDTH/2;
    memset(&state.block_status, 0xFF, num_blocks / 8 + (num_blocks % 8 ? 1 : 0));
    memset(&state.prev_block_status, 0xFF, num_blocks / 8 + (num_blocks % 8 ? 1 : 0));
    
    return state;
}

void run_breakout() {
    BreakoutGamestate state = init_gamestate();
    uint8_t points = 0; //state is reset across stages, but points carry over
    
    lcd_clear_buffer();
    draw_blocks(state.block_status);
    draw_ball(&state.ball);
    draw_paddle(state.paddle_x);
    lcd_display();
    set_led_from_points(points, 3 * num_blocks);
    
    while (1) {
        lcd_clear_buffer();
        if (joystick_pressed && last_joystick_direction == LEFT 
            && roundacc0(state.paddle_x) > (paddle_width - 1)/2 + paddle_speed) {
            state.paddle_x -= paddle_speed;
        } else if (joystick_pressed && last_joystick_direction == RIGHT 
            && roundacc0(state.paddle_x) < DISPLAY_WIDTH - 1 - (paddle_width - 1)/2 - paddle_speed) {
            state.paddle_x += paddle_speed;
        }
        move_ball(&state.ball);
        handle_collisions(&state.ball, state.block_status, state.paddle_x);
        
        if (bottom_collision(&state.ball)) {
            break;
        }
        
        draw_blocks(state.block_status);
        draw_ball(&state.ball);
        draw_paddle(state.paddle_x);
        display_ball(&state.ball);
        display_paddle(state.paddle_x);
        for (uint8_t i=0; i < num_blocks; i++) {
            if (get_block_status(i, state.block_status) != 
                    get_block_status(i, state.prev_block_status)) {
                display_block(i);
                // increase ball speed for each block hit
                state.ball.v.x *= ball_speed_increase_factor;
                state.ball.v.y *= ball_speed_increase_factor;
                points++;
                set_led_from_points(points, 3 * num_blocks);
                if (points % num_blocks == 0) {
                    lcd_gotoxy(6, DISPLAY_HEIGHT/8 - 3);
                    lcd_puts("next stage");
                    lcd_display();
                    wait_for_button();
                    AccVec old_v = state.ball.v;
                    state = init_gamestate();
                    state.ball.v = old_v;
                    while (state.ball.v.y > -2 ) {rotate_vec(&state.ball.v, 1);} 
                    lcd_clrscr();
                    draw_blocks(state.block_status);
                    lcd_display();
                }
            }
        }
        _delay_ms(16);
        memcpy(state.prev_block_status, state.block_status,
                sizeof(state.prev_block_status));
    }
    show_game_over_screen(points);
}
