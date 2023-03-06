#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "clock_frame.h"

#define NUM_PAGES 256 // Number of pages in secondary memory
#define NUM_CLK_FRAMES 8 // Number of clock frames representing the

int main(int argc, char **argv)
{
	int file = open("storage.txt", O_RDWR, S_IRUSR | S_IWUSR);
  if (file) {
    struct Clock* clock = setup_clock(32, file);
    printClock(clock);
    free_clock(clock);

    char* buff = (char*)malloc(PAGE_SIZE);
    memcpy(buff, "Test string", sizeof("Test string"));
    struct ClockFrame frame;
    frame.data = buff;
    frame.fd = file;
    frame.pageNo = 16;
    store(&frame);

    frame.pageNo = 15;
    load(&frame);
    printf("Success! - \"%s\"\n", (char*)frame.data);
    free(frame.data);

    frame.pageNo = 16;
    load(&frame);
    printf("Success! - \"%s\"\n", (char*)frame.data);
    free(frame.data);

    close(file);
  }
}
