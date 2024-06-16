#ifndef LIBSENSEHAT_JOYSTICK_H
#define LIBSENSEHAT_JOYSTICK_H

#define SENSE_JOYSTICK_NAME "Raspberry Pi Sense HAT Joystick"

#include <dirent.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <unistd.h>

typedef struct {
    int fd;
} sense_joystick_t;

sense_joystick_t *get_joystick(void);
void read_next_joystick_event(sense_joystick_t *joystick, int timeout,
                              void (*joystick_callback)(unsigned int event_code));
void free_joystick(sense_joystick_t *joystick);

#endif //LIBSENSEHAT_JOYSTICK_H
