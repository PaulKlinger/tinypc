#include "breakout.h"
#include "../mcc_generated_files/config/clock_config.h"
#include <util/delay.h>
#include <string.h>
#include "../lcd.h"
#include "../utilities.h"

#define num_blocks 24
#define initial_ball_speed 1.5 // speed of ball / frame
#define ball_speed_increase_factor 1.025 // factor speed increases by each block
#define ball_radius 2.5
#define ball_int_radius 2
#define paddle_width 25
#define paddle_height 3
#define paddle_speed 3.5


typedef struct {
    float x, y, vx, vy;
    float speed;
    // for partial display updates
    // tline means topmost display line intersecting the ball
    uint8_t prev_x, prev_tline; 
} Ball;

typedef struct {
    Ball ball;
    float paddle_x;
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
    ball->vx *= ball->speed / current_speed;
    ball->vy *= ball->speed / current_speed;
}

static void wall_collision(Ball *ball) {
    if (ball->x < ball_radius) {
        ball->y = ball->y - (ball->x - ball_radius) / ball->vx * ball->vy;
        ball->x = ball_radius;
        ball->vx *= -1;
    } else if (ball->x > DISPLAY_WIDTH - ball_radius) {
        ball->y = ball->y - (ball->x - DISPLAY_WIDTH + ball_radius) / ball->vx * ball->vy;
        ball->x = DISPLAY_WIDTH - ball_radius;
        ball->vx *= -1;
    } else if (ball->y < ball_radius) {
        ball->x = ball->x - (ball->y - ball_radius) / ball->vy * ball->vx;
        ball->y = ball_radius;
        ball->vy *= -1;
    } 
}

static bool bottom_collision(Ball *ball) {
    if (ball->y > DISPLAY_HEIGHT - ball_radius) {
        return true;
    }
    return false;
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

static struct BlockCoords calc_paddle_coords(float paddle_x) {
    struct BlockCoords ret;
    ret.x1 = round(paddle_x) - (paddle_width - 1) / 2;
    ret.y1 = DISPLAY_HEIGHT - 1 - paddle_height;
    ret.x2 = round(paddle_x) + (paddle_width - 1) / 2;
    ret.y2 = DISPLAY_HEIGHT - 1;
    return ret;
}

static void paddle_collision(Ball *ball, float paddle_x) {
    struct BlockCoords paddle_coords = calc_paddle_coords(paddle_x);
    if (ball_intersects_block(ball, paddle_coords)) {
        do {
            ball->x -= ball->vx / 10;
            ball->y -= ball->vy / 10;
        } while (ball_intersects_block(ball, paddle_coords));
        ball->vy *= -1;
        // The direction of the outgoing ball depends on the impact point
        // on the paddle. This makes the game more interesting and prevents
        // getting stuck in a loop.
        ball->vx += (ball->x - paddle_x) / (paddle_width);
        // make sure the ball speed stays the same
        normalize_ball_v(ball);
    }
}

static void handle_collisions(Ball *ball, uint8_t *block_status, float paddle_x) {
    wall_collision(ball);
    block_collision(ball, block_status);
    paddle_collision(ball, paddle_x);
}

static void move_ball(Ball *ball) {
    ball->prev_x = round(ball->x);
    ball->prev_tline = (round(ball->y) - ball_int_radius) / 8;
    ball->x += ball->vx;
    ball->y += ball->vy;
}

static void draw_paddle(float paddle_x){
    uint8_t xmin = round(paddle_x) - (paddle_width - 1) / 2;
    lcd_fillRect(xmin, DISPLAY_HEIGHT - 1 - paddle_height,
                 xmin + paddle_width, DISPLAY_HEIGHT - 1, 1);
}
static void display_paddle(float paddle_x) {
    uint8_t xmin = round(paddle_x) - (paddle_width - 1) / 2;
    lcd_display_block(xmin < ceil(paddle_speed) ? 0 : xmin - ceil(paddle_speed),
                      DISPLAY_HEIGHT / 8 - 1, paddle_width + 1 + 2 * ceil(paddle_speed));
}

static BreakoutGamestate init_gamestate() {
    Ball ball = {.x = DISPLAY_WIDTH/2, .y = (DISPLAY_HEIGHT - 6),
                 .vx = 3, .vy = -10, .speed=1};
    normalize_ball_v(&ball);
    BreakoutGamestate state;
    state.ball = ball;
    state.paddle_x = DISPLAY_WIDTH/2;
    memset(&state.block_status, 0xFF, num_blocks / 8 + (num_blocks % 8 ? 1 : 0));
    memset(&state.prev_block_status, 0xFF, num_blocks / 8 + (num_blocks % 8 ? 1 : 0));
    
    return state;
}

void run_breakout() {
    BreakoutGamestate state = init_gamestate();
    
    lcd_clear_buffer();
    draw_blocks(state.block_status);
    draw_ball(&state.ball);
    draw_paddle(state.paddle_x);
    lcd_display();
    wait_for_button();
    while (button_pressed);
    uint8_t points = 0;
    
    while (1) {
        lcd_clear_buffer();
        if (joystick_pressed && last_joystick_direction == LEFT 
            && round(state.paddle_x) > (paddle_width - 1)/2 + paddle_speed) {
            state.paddle_x -= paddle_speed;
        } else if (joystick_pressed && last_joystick_direction == RIGHT 
            && round(state.paddle_x) < DISPLAY_WIDTH - 1 - (paddle_width - 1)/2 - paddle_speed) {
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
                state.ball.speed *= ball_speed_increase_factor;
                normalize_ball_v(&state.ball);
                points++;
                if (points % num_blocks == 0) {
                    lcd_gotoxy(2, DISPLAY_HEIGHT/8 - 3);
                    lcd_puts("next stage");
                    lcd_display();
                    wait_for_button();
                    float old_speed = state.ball.speed;
                    state = init_gamestate();
                    state.ball.speed = old_speed;
                    normalize_ball_v(&state.ball);
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
