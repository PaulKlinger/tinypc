#include "utilities.h"

#include "light_ws2812.h"
#include "lcd.h"
#include "strings.h"


// Amount to divide led brightness by
// (For the tiny laptop the led is directly visible and needs to be a lot dimmer)

#ifdef THINKTINY
#define LED_DIM_FACTOR 30
#else
#define LED_DIM_FACTOR 1
#endif

void set_led(uint8_t r, uint8_t g, uint8_t b) {
    struct cRGB led[1];
    led[0].r=r / LED_DIM_FACTOR;
    led[0].g=g / LED_DIM_FACTOR;
    led[0].b=b / LED_DIM_FACTOR;
    ws2812_setleds(led,1);
}

void display_4x4_block(uint8_t x, uint8_t y) {
    lcd_display_block(x*4, y/2, 4);
}

// All of this debouncing stuff is horribly inefficient but at least it seems to work...
// Could save a lot of ram too...
volatile static uint8_t joystick_debounce_counter; 
volatile static uint8_t button_debounce_counter;


# define DEBOUNCE_THRESHOLD 4 /* x 8ms */
ISR(RTC_PIT_vect)
{
    if (!IO_PA4_get_level()){
        last_joystick_direction = RIGHT;
    } else if (!IO_PA5_get_level()){
        last_joystick_direction = LEFT;
    } else if (!IO_PA2_get_level()) {
        last_joystick_direction = UP;
    } else if (!IO_PA3_get_level()) {
        last_joystick_direction = DOWN;
    }
    bool joystick_currently_pressed = (!IO_PA4_get_level()||!IO_PA5_get_level()
            ||!IO_PA2_get_level()||!IO_PA3_get_level());
    if (joystick_currently_pressed != joystick_pressed){
        joystick_debounce_counter++;
        if (joystick_debounce_counter >= DEBOUNCE_THRESHOLD){
            joystick_pressed = joystick_currently_pressed;
            joystick_debounce_counter = 0;
        }
    } else {
        joystick_debounce_counter = 0;
    }
    
    bool button_currently_pressed = !IO_PA1_get_level();
    if (button_currently_pressed != button_pressed) {
        button_debounce_counter++;
        if (button_debounce_counter >= DEBOUNCE_THRESHOLD) {
            button_pressed = button_currently_pressed;
            button_debounce_counter = 0;
        }
    } 
    /* TRIGB interrupt flag has to be cleared manually */
    RTC.PITINTFLAGS = RTC_PI_bm;
}


void wait_for_button() {
    while (button_pressed);
    while (!button_pressed);
}

void show_game_over_screen(uint16_t points) {
    lcd_clear_buffer();
    lcd_gotoxy(6,2);
    lcd_puts_p(string_game_over);
    char points_str[5];
    ltoa(points, points_str, 10);
    lcd_gotoxy(6,3);
    lcd_puts(points_str);
    lcd_puts_p(string_points);
    lcd_gotoxy(3,5);
    lcd_puts_p(string_press_to_return);
    lcd_display();
    wait_for_button();
}

void set_led_from_points(uint16_t points, uint16_t max_points) {
    if (points > max_points) {
        points = max_points;
    }
    uint8_t r = roundacc0((1 - ((accum) points) / max_points) * 255);
    uint8_t g = roundacc0(((accum) points) / max_points * 255);
    set_led(r, g, 0);
}

accum roundacc0(accum x) {
    return roundfx(x,0);
}

uint8_t ceilacc8(accum x) {
    uint8_t f = x; // floor
    return  x > f ? f + 1 : f;
}

void rotate_vec(AccVec *vec, int8_t angle) {
    // Rotates vec in steps of 1 degree. Probably quite a lot of error because
    // of accum precision but should be good enough.
    // Rotating by 2 degree at a time would increase precision if I never
    // need to rotate in finer steps
    accum x_temp;
    bool dir = angle > 0;
    angle = abs(angle);
    for (; angle > 0; angle--) {
        x_temp = vec->x;
        vec->x = 0.999848K * vec->x - (dir ? 1 : -1) * 0.0174524K * vec->y;
        vec->y = (dir ? 1 : -1) * 0.0174524K * x_temp + 0.999848K * vec->y;
    }
}

AccVec add(AccVec a, AccVec b) {
    return (AccVec) {a.x + b.x, a.y + b.y};
}

uint8_t randint(uint8_t min, uint8_t max) {
    uint8_t ret = rand();
    while (ret < min || ret > max) ret = rand();
    return ret;
}


bool bitmatrix_get(BitMatrix matrix, uint8_t x, uint8_t y) {
    return matrix.data[y * matrix.byte_width + x / 8] & (1 << (x % 8));
}

void bitmatrix_set(BitMatrix matrix, uint8_t x, uint8_t y) {
    matrix.data[y * matrix.byte_width + x / 8] |= (1 << (x % 8));
}

void bitmatrix_unset(BitMatrix matrix, uint8_t x, uint8_t y) {
    matrix.data[y * matrix.byte_width + x / 8] &= ~(1 << (x % 8));
}
