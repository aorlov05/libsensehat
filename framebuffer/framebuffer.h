#ifndef LIBSENSEHAT_FRAMEBUFFER_H
#define LIBSENSEHAT_FRAMEBUFFER_H

#define SENSE_HAT_LENGTH 8
#define SENSE_FRAMEBUFFER_NAME "RPi-Sense FB"

#include <dirent.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

typedef struct {
    uint16_t pixel[SENSE_HAT_LENGTH][SENSE_HAT_LENGTH];
} sense_fbdata_t;

typedef struct {
    int fbfd;
    uint32_t buffer_size;
    sense_fbdata_t *fbdata;
} sense_framebuffer_t;

sense_framebuffer_t *get_sense_hat_framebuffer(void);
uint16_t rgb888_to_rgb565(uint8_t red, uint8_t green, uint8_t blue);
void set_pixel(sense_framebuffer_t *framebuffer, int x, int y, uint16_t color);
void clear_screen(sense_framebuffer_t *framebuffer);
void free_framebuffer(sense_framebuffer_t *framebuffer);

#endif //LIBSENSEHAT_FRAMEBUFFER_H
