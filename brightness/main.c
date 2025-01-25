#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>
#include <libudev.h>
#include <poll.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>

#define INOTIFY_EVENT_SIZE (sizeof (struct inotify_event))
#define INOTIFY_BUFF_SIZE 64

static bool get_float_value_from_file(const char *path,
                                      float *dst_val);

static void print_brightness_percent(const char *actual_path,
                                     const char *max_path);

static void usage(void) {
  fprintf(stderr, "brightness %s - read actual brightness value in non-blocking style.\n\n", VERSION);
  fprintf(stderr,
          "Usage: brightness [options]\n\
          \n\
          Options:\n\
          -a, --actual_brightness_path  \tpath to file with actual brightness string\n\
          -m, --max_brightness_path     \tpath to file with max brightness string\n\
          -h, --help                    \tprint this help.\n\
          -V, --version                 \tprint version and exit.\n\
          \n");
}
//////////////////////////////////////////////////////////////

int
main(int argc, char *argv[]) {
  static const char *default_abp = "/sys/class/backlight/intel_backlight/actual_brightness";
  static const char *default_mbp = "/sys/class/backlight/intel_backlight/max_brightness";
  char *actual_brightness_path = default_abp;
  char *max_brightness_path = default_mbp;

  int  opt_idx;
  size_t opt_len;

  static const struct option lopts[] = {
  {"actual_brightness_path", required_argument,  NULL, 'a'},
  {"max_brightness_path", required_argument,  NULL, 'm'},
  {"help", no_argument, NULL, 'h'},
  {"version", no_argument, NULL, 'V'},
  {NULL, 0, NULL, 0},
};

  while ((opt_idx = getopt_long(argc, argv, "a:m:hV", lopts, NULL)) != -1) {
    switch (opt_idx) {
      case 'a':
        opt_len = strlen(optarg);
        actual_brightness_path = malloc(opt_len + 1);
        strcpy(actual_brightness_path, optarg);
        break;
      case 'm':
        opt_len = strlen(optarg);
        max_brightness_path = malloc(opt_len + 1);
        strcpy(max_brightness_path, optarg);
        break;
      case 'h':
        usage();
        return 0;
      case 'V':
        printf("%s\n", VERSION);
        return 0;
      default:
        printf("something bad is happened. option index is out of bounds (%d %c)\n", opt_idx, (char)opt_idx);
        return -1;
    }
  }

  print_brightness_percent(actual_brightness_path,
                           max_brightness_path);

  struct udev *udev = udev_new();
  if (!udev) {
    fprintf(stderr, "creating udev object\n");
    return EXIT_FAILURE;
  }

  struct udev_monitor *mon = udev_monitor_new_from_netlink(udev, "udev");
  if (!mon) {
    fprintf(stderr, "creating udev monitor\n");
    return EXIT_FAILURE;
  }

  if (udev_monitor_filter_add_match_subsystem_devtype(mon, "backlight", NULL) < 0) {
    fprintf(stderr, "adding filter to udev monitor\n");
    udev_monitor_unref(mon);
    udev_unref(udev);
    return EXIT_FAILURE;
  }

  if (udev_monitor_filter_update(mon) < 0) {
    fprintf(stderr, "updating udev monitor filter\n");
    return EXIT_FAILURE;
  }

  int monfd = udev_monitor_get_fd(mon);
  if (monfd < 0) {
    fprintf(stderr, "getting udev monitor fd\n");
    return EXIT_FAILURE;
  }

  struct pollfd fds[1];
  fds[0].fd = monfd;
  fds[0].events = POLLIN;

  while (1) {
    int ret = poll(fds, 1, -1);
    if (ret < 0) {
      perror("poll failed");
      break;
    }

    if (fds[0].revents & POLLIN) {
      struct udev_device *dev = udev_monitor_receive_device(mon);
      if (dev) {
        const char *action = udev_device_get_action(dev);

        if (strncmp(action, "change", 6) == 0) {
          print_brightness_percent(actual_brightness_path,
                                   max_brightness_path);
        }
        udev_device_unref(dev);
      }
    }
  }

  udev_monitor_unref(mon);
  udev_unref(udev);

  //actually when we receive some signal (like sigterm or something like that)
  //we will be here.

  if (actual_brightness_path != default_abp)
    free(actual_brightness_path);
  if (max_brightness_path != default_mbp)
    free(max_brightness_path);
  return 0;
}
//////////////////////////////////////////////////////////////

bool
get_float_value_from_file(const char *path,
                          float *dst_val) {
#define BUFF_SIZE 16
  char buff[BUFF_SIZE] = {0};
  FILE *f = fopen(path, "r");
  bool result = false;
  if (f == NULL) {
    perror("failed to open file");
    return false;
  }

  do {
    if (!(fread((void*) buff, 1, BUFF_SIZE, f))) {
      perror("failed to read file");
      break;
    }

    char *unused;
    *dst_val = strtof(buff, &unused);
    result = true;
  } while (0);

  fclose(f);
  return result;
}
//////////////////////////////////////////////////////////////

void
print_brightness_percent(const char *actual_path,
                         const char *max_path) {
  float curr, max;
  const char *parr[] = {actual_path, max_path};
  float *farr[] = {&curr, &max};

  for (size_t i = 0; i < 2; ++i) {
    bool success = get_float_value_from_file(parr[i], farr[i]);\
    if (!success) {
      printf("\xF0\x9F\x94\x86"); //brightness symbol 🔆
      printf(": NA\n");
      fflush(stdout); // because we use select(), ant it blocks stdout
      return;
    }
  }

  printf("\xF0\x9F\x94\x86"); //brightness symbol 🔆
  printf(": %02.f%%\n", (curr / max) * 100.f);
  fflush(stdout); // because we use select(), ant it blocks stdout
}
//////////////////////////////////////////////////////////////
