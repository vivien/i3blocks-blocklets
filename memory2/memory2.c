#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/sysinfo.h>

#define RED "#FF7373"
#define ORANGE "#FFA500"


typedef unsigned long ulong;
typedef unsigned int uint;
typedef struct sysinfo sysinfo_t;

struct mem_stats_sg
{
  long long total;
  long long free;
  long long cache;
} typedef mem_stats_sg;


typedef struct {
  char bytes[4];
  int size;
} utf8_char;


#define BYTE_ONE 0x80   // 10000000
#define BYTE_TWO 0xC0   // 11000000
#define BYTE_THREE 0xE0 // 11100000
#define BYTE_FOUR 0xF0  // 11110000
#define BYTE_FIVE 0xF8  // 11111000


uint utf8_char_count(const char* str)
{
  uint count = 0;
  while(*str) {
    count += (*str & BYTE_TWO) != BYTE_ONE;
    str++;
  }
  return count;
}


void load_bar_chars(utf8_char* bar_chars, uint count, char* characters) {
  char* c = characters;
  for (uint i = 0; i < count; i++) {
    int size = 0;
    utf8_char* b = bar_chars + i;
    if ( !c[0] ) {
      // error
      printf("FAILED TO LOAD CHARS CORRECT");
    } else if (!(c[0] & BYTE_ONE)) {
      // character is one byte
      size = 1;
    } else if ( (c[0] & BYTE_THREE) == BYTE_TWO ) {
      size = 2;
    } else if ( (c[0] & BYTE_FOUR) == BYTE_THREE) {
      size = 3;
    } else if ( (c[0] & BYTE_FIVE) == BYTE_FOUR) {
      size = 4;
    }
    
    for (uint j = 0; j < size; j++)
     b->bytes[j] = c[j];

    b->size = size;
    
    c += size;
  }
}

int clamp(int value, int min, int max) {
  return value < min ? min : (value > max ? max : value);
}

/**
 * The idea for this solution came from libstatgrab
 * https://github.com/libstatgrab/libstatgrab
 */
void get_mem_stats(mem_stats_sg* mem_stats_buf, int ignore_swap)
{
  #define LINE_BUF_SIZE 256
  char *line_ptr, line_buf[LINE_BUF_SIZE];
  long long value;
  FILE *f = fopen("/proc/meminfo", "r");
  if(f != NULL)
  {

    #define MEM_TOTAL_PREFIX    "MemTotal:"
    #define MEM_FREE_PREFIX     "MemFree:"
    #define MEM_CACHED_PREFIX   "Cached:"
    while ((line_ptr = fgets(line_buf, sizeof(line_buf), f)) != NULL)
    {
      if(sscanf(line_buf, "%*s %lld kB", &value) != 1)
        continue;

      if(strncmp(line_buf, MEM_TOTAL_PREFIX, sizeof(MEM_TOTAL_PREFIX) - 1) == 0)
        mem_stats_buf->total = value;
      else if(strncmp(line_buf, MEM_FREE_PREFIX, sizeof(MEM_FREE_PREFIX) - 1) == 0)
        mem_stats_buf->free = value;
      else if(strncmp(line_buf, MEM_CACHED_PREFIX, sizeof(MEM_CACHED_PREFIX) - 1) == 0)
        mem_stats_buf->cache = value;
    }

    if(ignore_swap == 1)
      mem_stats_buf->free += mem_stats_buf->cache;

    mem_stats_buf->total *= 1024;
    mem_stats_buf->free *= 1024;
    mem_stats_buf->cache *= 1024;

    #undef MEM_TOTAL_PREFIX
    #undef MEM_FREE_PREFIX
    #undef MEM_CACHED_PREFIX

    fclose(f);
    
  }
}

int main(int argc, char *argv[])
{
  // load environment variables
  char *characters = "";
  uint bar_size = 10;
  char *envvar = NULL;
  int warning = 50;
  int critical = 80;
  char* color_warning = ORANGE;
  char* color_critical = RED;
  int ignore_swap = 0;
  
  mem_stats_sg* mem_stats_buf = malloc(sizeof(mem_stats_sg));

  envvar = getenv("bar_chars");
  if (envvar)
    characters = envvar;
  envvar = getenv("bar_size");
  if (envvar)
    bar_size = atoi(envvar);
  envvar = getenv("critical");
  if (envvar)
    critical = atoi(envvar);
  envvar = getenv("warning");
  if (envvar)
    warning = atoi(envvar);
  envvar = getenv("color_warning");
  if (envvar)
    color_warning = envvar;
  envvar = getenv("color_critical");
  if (envvar)
    color_critical = envvar;
  envvar = getenv("show_available");
  if(envvar)
    ignore_swap = (strcmp(envvar, "false")) ? 1 : 0;
  
  uint count = utf8_char_count(characters);
  utf8_char* bar_chars = (utf8_char*)malloc(count * sizeof(utf8_char));
  
  load_bar_chars(bar_chars, count, characters);

  // allocate the maximun size possible
  int buffer_size = (bar_size * 4) + 1;
  char* buffer = (char*)malloc(buffer_size);
  
  
  uint t = 1;
  while (1) {
    /**
     * Illustration by @mwittig from GitHub:
     * https://github.com/pimatic/pimatic-sysinfo/issues/19#issuecomment-478371045
     * 
     * |------------------------------------------------|
     * |                      R A M                     |
     * |---------------|--------------------------------|
     * |               |     available (6) estimated    |
     * |---------------|----------------------|---------|
     * |  active (2)   | buffers/cache (5)    | free (3)|
     * |--------------------------------------|---------|
     * |                 total (1)                      |
     * |------------------------------------------------|
     * 
     * This illustrations shows that if we don't ignore the cache, we
     * always get almost 100% memory usage, due to how memory is handled.
     */
    
    get_mem_stats(mem_stats_buf, ignore_swap);

    long total = (long)mem_stats_buf->total;
    long free = (long)mem_stats_buf->free;
    long usage = total - free;

    float percent = 100 * ((float)usage / total);
    float bar_percent = percent;

    memset(buffer, 0, buffer_size);
    
    //printf("%ld/%ld   %f  ", usage, total, percent);
    char* write_point = buffer;

    float section_size = 100.0 / bar_size; 
    for (uint i = 0; i < bar_size; i++) {
      int section_val = clamp((int)bar_percent, 0, count-1);
      utf8_char u_char = bar_chars[section_val];

      for (uint j = 0; j < u_char.size; j++)
	write_point[j] = u_char.bytes[j];

      bar_percent -= section_size;
      write_point += u_char.size;
    }

    if (critical != 0 && percent > critical) {
      printf("<span color='%s'>", color_critical);
    } else if (warning != 0 && percent > warning) {
      printf("<span color='%s'>", color_warning);
    } else {
      printf("<span>");
    }

    const float byte_to_gb = 1024 * 1024 * 1024;
    
    float usage_gb = usage / byte_to_gb;
    float total_gb = total / byte_to_gb;
    
    printf("%s %4.1fG/%4.1fG (%i%%)</span>\n", buffer, usage_gb, total_gb, (int)percent);
    fflush(stdout);

    sleep(t);
  }
  free(buffer);
  free(bar_chars);
  free(mem_stats_buf);
  return EXIT_SUCCESS;
}
