#include "framebuffer/framebuffer.h"

int main(void) {
    sense_framebuffer_t *framebuffer = get_sense_hat_framebuffer();
    if (framebuffer == NULL) {
        fprintf(stderr, "Could not find Sense Hat framebuffer");
        exit(1);
    }

    set_pixel(framebuffer, 1, 1, rgb888_to_rgb565(255, 255, 0));
    usleep(5000000);

    clear_screen(framebuffer);
    free_framebuffer(framebuffer);
    return 0;
}
