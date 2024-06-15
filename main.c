#include "framebuffer/framebuffer.h"
#include "joystick/joystick.h"

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

    set_pixel(framebuffer, 2, 2, rgb888_to_rgb565(255, 0, 255));
    usleep(1000000);

    clear_screen(framebuffer);
    free_framebuffer(framebuffer);
    free_joystick(joystick);
    return 0;
}
