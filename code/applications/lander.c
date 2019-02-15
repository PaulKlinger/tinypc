#include "breakout.h"
#include "../mcc_generated_files/config/clock_config.h"
#include <util/delay.h>
#include "../lcd.h"
#include "../utilities.h"


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
}

static accum  __attribute__ ((noinline)) line_height_at_x(u8Vec line_p1, u8Vec line_p2, accum x) {
    return ((accum) line_p2.y - line_p1.y) / (line_p2.x - line_p1.x) * (x - line_p1.x) + line_p1.y;
}
static bool point_terrain_collision(AccVec p, struct Terrain *terrain) {
    if (p.x <= landing_pad_width && p.y > DISPLAY_HEIGHT - 1 ) return true;
    if (p.x > landing_pad_width && p.x <= terrain->p1.x &&
        p.y > line_height_at_x((u8Vec){landing_pad_width, DISPLAY_HEIGHT - 1}, terrain->p1, p.x)) return true;
    if (p.x > terrain->p1.x && p.x <= terrain->p2.x &&
        p.y > line_height_at_x(terrain->p1, terrain->p2, p.x)) return true;
    if (p.x > terrain->p2.x && p.x <= terrain->p3.x &&
        p.y > line_height_at_x(terrain->p2, terrain->p3, p.x)) return true;
    if (p.x > terrain->p3.x && p.x <= DISPLAY_WIDTH - 1 - landing_pad_width &&
        p.y > line_height_at_x(terrain->p3, (u8Vec){DISPLAY_WIDTH - 1 - landing_pad_width, terrain->landing_y}, p.x)) return true;
    if (p.x > DISPLAY_WIDTH - 1 - landing_pad_width && p.y > terrain->landing_y ) return true;
    return false;
}

static bool lander_terrain_collision(struct Lander *lander, struct Terrain *terrain) {
    if (lander->pos.x < -14 || lander->pos.x >= DISPLAY_WIDTH + 14 ||
        lander->pos.y <= - 14 || lander-> pos.y >= DISPLAY_HEIGHT ) return true;
    return (point_terrain_collision(add(lander->t1p1, lander->pos), terrain) ||
            point_terrain_collision(add(lander->t1p2, lander->pos), terrain) ||
            point_terrain_collision(add(lander->t1p3, lander->pos), terrain));
}

void run_lander(){
    struct Lander lander = {.pos = {7, DISPLAY_HEIGHT - 1 - 5},
                            .v = {0, 0}, .dir = {0, -1},
                            .t1p1 = {0, -10}, .t1p2 = {5, 5}, .t1p3 = {-5, 5},
                            .t2p3 = {0, -1}};
    
    struct Terrain terrain = {.p1 = {20, 50}, .p2 = {70, 20}, .p3 = {100, 40},
                              .landing_y = 30};
    lcd_clear_buffer();
    draw_lander(&lander);
    draw_terrain(&terrain);
    lcd_display();
    while (button_pressed);
    while (!button_pressed && !joystick_pressed); 
    while (!button_pressed) {
        lcd_clear_buffer();
        lander.pos.x += lander.v.x;
        lander.pos.y += lander.v.y;
        lander.v.y += grav_accell;
        lander.thrust_on = false;
        if (joystick_pressed) {
            switch (last_joystick_direction) {
                case LEFT:
                    rotate_lander(&lander, -4);
                    break;
                case RIGHT:
                    rotate_lander(&lander, 4);
                    break;
                case UP:
                    lander.v.x += lander.dir.x * thrust_accell;
                    lander.v.y += lander.dir.y * thrust_accell;
                    lander.thrust_on = true;
                    break;
                case DOWN: // to get rid of warning, no code generated
                    break;
            }
        }
        draw_lander(&lander);
        draw_terrain(&terrain);
        if (lander_terrain_collision(&lander, &terrain)) {
            lcd_gotoxy(3, 0);
            lcd_puts("crashed!");
            lcd_display();
            wait_for_button();
            return;
        }
        lcd_display();
        _delay_ms(10);
    }
}