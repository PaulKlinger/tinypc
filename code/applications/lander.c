#include "breakout.h"
#include "../mcc_generated_files/config/clock_config.h"
#include <util/delay.h>
#include "../lcd.h"
#include "../utilities.h"
#include "../strings.h"
#include <stdfix.h>


#define grav_accell 0.005K
#define thrust_accell 0.01K
#define landing_pad_width 14

struct Lander {
    AccVec pos, v, dir;
    AccVec t1p1, t1p2, t1p3, t2p3;
    bool thrust_on;
};

struct Terrain {
    u8Vec p1, p2, p3;
    uint8_t landing_y;
};

struct LanderGamestate {
    struct Lander lander;
    struct Terrain terrain;
};


static void draw_lander(struct Lander *lander) {
    lcd_fillTriangle(lander->t1p1.x + lander->pos.x, lander->t1p1.y + lander->pos.y,
                lander->t1p2.x + lander->pos.x, lander->t1p2.y + lander->pos.y,
                lander->t1p3.x + lander->pos.x, lander->t1p3.y + lander->pos.y,
                1);
    lcd_fillTriangle(lander->t1p3.x + lander->pos.x, lander->t1p3.y + lander->pos.y,
                lander->t1p2.x + lander->pos.x, lander->t1p2.y + lander->pos.y,
                lander->t2p3.x + lander->pos.x, lander->t2p3.y + lander->pos.y,
                0);
    
    if (lander->thrust_on) {
        AccVec plume_dir = lander->dir;
        plume_dir.x *= -8;
        plume_dir.y *= -8;
        rotate_vec(&plume_dir, -35);
        for (uint8_t i = 5; i>0; i--) {
            rotate_vec(&plume_dir, 10);
            lcd_drawLine(lander->t2p3.x + lander->pos.x, lander->t2p3.y + lander->pos.y,
                         lander->t2p3.x + lander->pos.x + plume_dir.x,
                         lander->t2p3.y + lander->pos.y + plume_dir.y, 1);
        }
    }
}

static void rotate_lander(struct Lander *lander, int8_t angle) {
    rotate_vec(&lander->dir, angle);
    rotate_vec(&lander->t1p1, angle);
    rotate_vec(&lander->t1p2, angle);
    rotate_vec(&lander->t1p3, angle);
    rotate_vec(&lander->t2p3, angle);
}

static void draw_terrain(struct Terrain *terrain) {
    lcd_drawLine(0, DISPLAY_HEIGHT-1, landing_pad_width, DISPLAY_HEIGHT-1,1);
    lcd_drawLine(landing_pad_width, DISPLAY_HEIGHT-1, terrain->p1.x, terrain->p1.y, 1);
    lcd_drawLine(terrain->p1.x, terrain->p1.y, terrain->p2.x, terrain->p2.y, 1);
    lcd_drawLine(terrain->p2.x, terrain->p2.y, terrain->p3.x, terrain->p3.y, 1);
    lcd_drawLine(terrain->p3.x, terrain->p3.y, DISPLAY_WIDTH - 1 - landing_pad_width, terrain->landing_y, 1);
    lcd_drawLine(DISPLAY_WIDTH - 1 - landing_pad_width, terrain->landing_y, DISPLAY_WIDTH - 1, terrain->landing_y, 1);
    lcd_drawLine(DISPLAY_WIDTH - 1 - landing_pad_width, terrain->landing_y+1, DISPLAY_WIDTH - 1, terrain->landing_y+1, 1);
}

static accum  __attribute__ ((noinline)) line_height_at_x(u8Vec line_p1, u8Vec line_p2, accum x) {
    // add 2 for some buffer (the displayed triangles don't reach to the points)
    return 2 + ((accum) line_p2.y - line_p1.y) / (line_p2.x - line_p1.x) * (x - line_p1.x) + line_p1.y;
}
static bool point_terrain_collision(AccVec p, struct Terrain *terrain) {
    if (p.x <= landing_pad_width && p.y > 1 + DISPLAY_HEIGHT ) return true;
    if (p.x > landing_pad_width && p.x <= terrain->p1.x &&
        p.y > line_height_at_x((u8Vec){landing_pad_width, DISPLAY_HEIGHT - 1}, terrain->p1, p.x)) return true;
    if (p.x > terrain->p1.x && p.x <= terrain->p2.x &&
        p.y > line_height_at_x(terrain->p1, terrain->p2, p.x)) return true;
    if (p.x > terrain->p2.x && p.x <= terrain->p3.x &&
        p.y > line_height_at_x(terrain->p2, terrain->p3, p.x)) return true;
    if (p.x > terrain->p3.x && p.x <= DISPLAY_WIDTH - 1 - landing_pad_width &&
        p.y > line_height_at_x(terrain->p3, (u8Vec){DISPLAY_WIDTH - 1 - landing_pad_width, terrain->landing_y}, p.x)) return true;
    if (p.x > DISPLAY_WIDTH - 1 - landing_pad_width && p.y > 2 + terrain->landing_y ) return true;
    return false;
}

static bool lander_terrain_collision(struct LanderGamestate *s) {
    if (s->lander.pos.x < -14 || s->lander.pos.x >= DISPLAY_WIDTH + 14 ||
        s->lander.pos.y <= - 14 || s->lander.pos.y >= DISPLAY_HEIGHT ) return true;
    return (point_terrain_collision(add(s->lander.t1p1, s->lander.pos), &s->terrain) ||
            point_terrain_collision(add(s->lander.t1p2, s->lander.pos), &s->terrain) ||
            point_terrain_collision(add(s->lander.t1p3, s->lander.pos), &s->terrain));
}

static struct LanderGamestate new_stage() {
    u8Vec p1 = {randrange(landing_pad_width+4, 44), randrange(20, DISPLAY_HEIGHT - 1)};
    u8Vec p2 = {randrange(p1.x, 74), randrange(20, DISPLAY_HEIGHT - 1)};
    u8Vec p3 = {randrange(p2.x, 104), randrange(20, DISPLAY_HEIGHT - 1)};
    return (struct LanderGamestate) {
        .lander = (struct Lander){.pos = {7, DISPLAY_HEIGHT - 1 - 5},
                                  .v = {0, 0}, .dir = {0, -1},
                                  .t1p1 = {0, -10}, .t1p2 = {5, 5},
                                  .t1p3 = {-5, 5}, .t2p3 = {0, -1}},
        .terrain = (struct Terrain){.p1 = p1, .p2 = p2, .p3 = p3,
                                    .landing_y = randrange(20, DISPLAY_HEIGHT - 2)
                                  }
    };
}

static bool lander_landed(struct LanderGamestate *s) {
    if (s->lander.pos.x >= DISPLAY_WIDTH - 1 - landing_pad_width / 2 - 5
        && s->lander.pos.x <= DISPLAY_WIDTH - 1 - landing_pad_width / 2 + 5
        && s->lander.pos.y > s->terrain.landing_y - 5
        && s->lander.dir.y < -0.97K /* something like 10-15° tilt*/
        && absfx(s->lander.v.x) < 0.1K && absfx(s->lander.v.y) < 0.1K ) return true;
    return false;
}

void run_lander(){
    
    uint8_t points = 0;
    set_led_from_points(points, 4);
    while (true) {
        struct LanderGamestate state = new_stage();
         lcd_clear_buffer();
        draw_lander(&(state.lander));
        draw_terrain(&(state.terrain));
        lcd_display();
        while (button_pressed);
        while (!button_pressed && !joystick_pressed); 
        while (!button_pressed) {
            lcd_clear_buffer();
            state.lander.pos.x += state.lander.v.x;
            state.lander.pos.y += state.lander.v.y;
            state.lander.v.y += grav_accell;
            state.lander.thrust_on = false;
            if (joystick_pressed) {
                switch (last_joystick_direction) {
                    case LEFT:
                        rotate_lander(&(state.lander), -4);
                        break;
                    case RIGHT:
                        rotate_lander(&(state.lander), 4);
                        break;
                    case UP:
                        state.lander.v.x += state.lander.dir.x * thrust_accell;
                        state.lander.v.y += state.lander.dir.y * thrust_accell;
                        state.lander.thrust_on = true;
                        break;
                    case DOWN: // to get rid of warning, no code generated
                        break;
                }
            }
            draw_lander(&(state.lander));
            draw_terrain(&(state.terrain));
            if (lander_landed(&state)) {
                points++;
                lcd_gotoxy(4, 0);
                lcd_puts_p(string_next_stage);
                lcd_display();
                set_led_from_points(points, 4);
                wait_for_button();
                break;
            } else if (lander_terrain_collision(&state)) {
                lcd_gotoxy(3, 0);
                lcd_puts("crashed!");
                lcd_gotoxy(3,1);
                lcd_puts_p(string_game_over);
                char points_str[3];
                itoa(points, points_str, 10);
                lcd_gotoxy(3,2);
                lcd_puts(points_str);
                lcd_puts_p(string_points);
                lcd_display();
                wait_for_button();
                return;
            }
            lcd_display();
            _delay_ms(16);
        }
    }
}