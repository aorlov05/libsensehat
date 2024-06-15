#include "framebuffer.h"

/**
 * Takes in a directory entry for a device file and returns if it is a framebuffer
 * @param dir_name A directory entry (e.g. fb0)
 * @return If it is a framebuffer
 */
int is_framebuffer(char *dir_name) {
    return strncmp(dir_name, "fb", 2) == 0;
}

/**
 * Takes in the string representation of a directory entry and allocates a sense_framebuffer_t object
 * with its fbdata field set to a mapping of the framebuffer.
 * If the inputted directory is not a RPi Sense Hat framebuffer, this returns NULL.
 * This allocates a sense_framebuffer_t object which must be freed with a call to free_framebuffer().
 * @param dir A directory field
 * @return A sense_framebuffer_t object on success, NULL on failure
 */
sense_framebuffer_t *map_sense_hat_framebuffer(char *dir) {
    int fbfd = open(dir, O_RDWR);
    if (fbfd < 0) {
        fprintf(stderr, "Could not open framebuffer device %s\n", dir);
        return NULL;
    }

    struct fb_fix_screeninfo info;
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &info) == -1) {
        fprintf(stderr, "Could not read fixed screen info with ioctl %s\n", dir);
        goto error_exit;
    }

    if (strcmp(SENSE_FRAMEBUFFER_NAME, info.id) != 0) {
        goto error_exit;
    }

    sense_framebuffer_t *framebuffer = malloc(sizeof(sense_framebuffer_t));
    if (framebuffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        goto error_exit;
    }

    framebuffer->fbfd = fbfd;
    framebuffer->buffer_size = info.smem_len;
    // Maps the Sense Hat LED bitmap into memory
    framebuffer->fbdata = mmap(0, framebuffer->buffer_size,
                               PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (framebuffer->fbdata == MAP_FAILED) {
        free(framebuffer);
        fprintf(stderr, "Mapping buffer into memory failed\n");
        goto error_exit;
    }

    return framebuffer;
    // Cleans open file descriptor if error accessing framebuffer
    error_exit:
        close(fbfd);
        return NULL;
}

/**
 * Searches through every framebuffer device connected and returns the
 * a sense_framebuffer_t object which is mapped
 * This calls map_sense_hat_framebuffer which allocates memory for a sense_framebuffer object
 * that must be freed with a call to free_framebuffer().
 * @return A sense_framebuffer_t object on success, NULL on failure
 */
sense_framebuffer_t *get_sense_hat_framebuffer(void) {
    DIR *dir = opendir("/dev");
    if (dir == NULL) {
        fprintf(stderr, "Could not open /dev directory");
        return NULL;
    }

    struct dirent *entry;
    sense_framebuffer_t *framebuffer = NULL;
    for (entry = readdir(dir); entry != NULL; entry = readdir(dir)) {
        if (!is_framebuffer(entry->d_name)) {
            continue;
        }

        char fb_device[256];
        snprintf(fb_device, sizeof(fb_device), "/dev/%s", entry->d_name);
        framebuffer = map_sense_hat_framebuffer(fb_device);
        if (framebuffer != NULL) {
            break;
        }
    }

    closedir(dir);
    return framebuffer;
}

/**
 * Converts a standard RGB value to a 16 bit representation that can be read
 * by the RPi Sense Hat framebuffer.
 * https://stackoverflow.com/questions/11471122/rgb888-to-rgb565-bit-shifting
 * @param red The 8 bit red value
 * @param green The 8 bit green value
 * @param blue The 8 bit blue value
 * @return A 16 bit representation of a 32 bit color
 */
uint16_t rgb888_to_rgb565(uint8_t r, uint8_t g, uint8_t b) {
    uint16_t red = (r >> 3) & 0x1F; // 5 bits for red
    uint16_t green = (g >> 2) & 0x3F; // 6 bits for green
    uint16_t blue = (b >> 3) & 0x1F; // 5 bits for blue
    return (red << 11) | (green << 5) | blue;
}

/**
 * Sets the color of a pixel on the RPi Sense Hat.
 * If the x and y are outside of the bounds, does nothing.
 * @param framebuffer The RPi Sense Hat framebuffer
 * @param x The x position of the pixel to change
 * @param y The y position of the pixel to change
 * @param color An 16 bit representation of a color
 */
void set_pixel(sense_framebuffer_t *framebuffer, int x, int y, uint16_t color) {
    if (x < 0 || x >= SENSE_HAT_LENGTH || y < 0 || y >= SENSE_HAT_LENGTH) {
        fprintf(stderr, "Cannot set pixel outside Sense Hat bounds");
        return;
    }

    framebuffer->fbdata->pixel[x][y] = color;
}

/**
 * Clears the screen by setting all mapped framebuffer data to zeros.
 * @param framebuffer The RPi Sense Hat framebuffer
 */
void clear_screen(sense_framebuffer_t *framebuffer) {
    memset(framebuffer->fbdata, 0, framebuffer->buffer_size);
}

/**
 * Unmaps the framebuffer data from memory and deallocates the framebuffer object.
 * @param framebuffer The RPi Sense Hat framebuffer
 */
void free_framebuffer(sense_framebuffer_t *framebuffer) {
    munmap(framebuffer->fbdata, framebuffer->buffer_size);
    close(framebuffer->fbfd);
    free(framebuffer);
}
