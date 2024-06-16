#include "include/sense.h"

static int running = 1;
int x = 0, y = 0;

void joystick_callback(unsigned int code);

int main(void) {
    sense_framebuffer_t *framebuffer = get_sense_hat_framebuffer();
    if (framebuffer == NULL) {
        fprintf(stderr, "Could not find Sense Hat framebuffer\n");
        exit(1);
    }

    sense_joystick_t *joystick = get_joystick();
    if (joystick == NULL) {
        fprintf(stderr, "Could not find Sense Hat joystick\n");
        exit(1);
    }

    while (running) {
        clear_screen(framebuffer);
        set_pixel(framebuffer, x, y, rgb888_to_rgb565(255, 0, 255));
        read_next_joystick_event(joystick, -1, joystick_callback);
    }

    clear_screen(framebuffer);
    free_framebuffer(framebuffer);
    free_joystick(joystick);
    return 0;
}

void joystick_callback(unsigned int code) {
    switch (code) {
        case KEY_UP:
            printf("Up!\n");
            x--;
            break;
        case KEY_DOWN:
            printf("Down!\n");
            x++;
            break;
        case KEY_LEFT:
            printf("Left!\n");
            y--;
            break;
        case KEY_RIGHT:
            printf("Right!\n");
            y++;
            break;
        case KEY_ENTER:
            printf("Pushed in!\n");
            running = 0;
            break;
        default:
            printf("What is this? %d\n", code);
    }
}
