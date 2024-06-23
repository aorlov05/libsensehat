#include "../include/joystick.h"

/**
 * Takes a directory entry for an input device file and returns if it can read events
 * @param dir_name A directory entry
 * @return If the directory is an event device
 */
int is_event_device(char *dir) {
    return strncmp(dir, "event", 5) == 0;
}

/**
 * Takes a string representation of the full path to an input device and
 * allocates a sense_joystick_t object with its file descriptor if it is a joystick device.
 * If the inputted directory is not a RPi Sense Hat joystick, this returns NULL.
 * This allocates a sense_joystick_t object which must be freed with a call to free_joystick().
 * @param dir A path to an event device
 * @return A sense_joystick_t object on success, NULL on failure
 */
sense_joystick_t *map_joystick(char *dir) {
    int fd = open(dir, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Could not open input device file %s\n", dir);
        return NULL;
    }

    char device_name[256];
    // Copies the event device's name into char device_name
    if (ioctl(fd, EVIOCGNAME(sizeof(device_name)), device_name) == -1) {
        fprintf(stderr, "Could not read device name %s\n", dir);
        close(fd);
        return NULL;
    }

    if (strcmp(SENSE_JOYSTICK_NAME, device_name) != 0) {
        close(fd);
        return NULL;
    }

    sense_joystick_t *joystick = malloc(sizeof(sense_joystick_t));
    joystick->fd = fd;
    return joystick;
}

/**
 * Searches through every event device and returns a sense_joystick_t object
 * containing the file descriptor of the RPi Sense Hat joystick.
 * This returns a sense_joystick_t object on success which must be freed with free_joystick().
 * @return A sense_joystick_t object on success, NULL on failure
 */
sense_joystick_t *get_joystick(void) {
    DIR *dir = opendir("/dev/input");
    if (dir == NULL) {
        fprintf(stderr, "Could not open /dev/input directory\n");
        return NULL;
    }

    struct dirent *entry;
    sense_joystick_t *joystick = NULL;
    for (entry = readdir(dir); entry != NULL; entry = readdir(dir)) {
        if (!is_event_device(entry->d_name)) {
            continue;
        }

        char input_device[256];
        snprintf(input_device, sizeof(input_device), "/dev/input/%s", entry->d_name);
        joystick = map_joystick(input_device);
        if (joystick != NULL) {
            break;
        }
    }

    closedir(dir);
    return joystick;
}

/**
 * Takes a joystick file descriptor and waits for a key press for timeout milliseconds.
 * If the timeout is negative, this will wait for an infinite amount of time until key press.
 * If a key is pressed, this passes the key code to the joystick_callback function.
 * @param joystick The RPi Sense Hat joystick
 * @param timeout How long to wait for key press in milliseconds
 * @param joystick_callback A function that takes the pressed event code and is called on key press
 */
void read_next_joystick_event(sense_joystick_t *joystick, int timeout,
                              void (*joystick_callback)(unsigned int event_code)) {
    struct pollfd fds;
    fds.fd = joystick->fd;
    fds.events = POLLIN;
    // Waits timeout time for an event, and checks if the file descriptor has returned events
    if (poll(&fds, 1, timeout) && (fds.revents & POLLIN)) {
        struct input_event event;
        ssize_t input = read(joystick->fd, &event, sizeof(event));
        if (input != sizeof(event)) {
            fprintf(stderr, "Could not read input event %zd\n", input);
            return;
        }

        // If key was pressed and callback function was passed in
        if (event.type == EV_KEY && event.value == 1 && joystick_callback != NULL) {
            joystick_callback(event.code);
        }
    }
}

/**
 * Closes the file descriptor associated with the joystick and deallocates the joystick object.
 * @param joystick The RPi Sense Hat joystick
 */
void free_joystick(sense_joystick_t *joystick) {
    close(joystick->fd);
    free(joystick);
}
