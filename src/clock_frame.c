#include "clock_frame.h"
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

size_t getPageNo(size_t addr) {
    return (addr >> 12) & 0x000000ff;
}

size_t getOffset(size_t addr) {
    return addr & 0x00000fff;
}

void load(struct ClockFrame* frame) {
  fprintf(stderr, "load(...) has been called\r\n");
  frame->data = malloc(PAGE_SIZE);
  pread(frame->fd, frame->data, PAGE_SIZE, frame->pageNo * PAGE_SIZE);
  frame->rBit = SET;
  frame->mBit = CLEAN;
}

void store(struct ClockFrame* frame) {
  fprintf(stderr, "store(...) has been called\r\n");
  pwrite(frame->fd, frame->data, PAGE_SIZE, frame->pageNo * PAGE_SIZE);
  free(frame->data);
  frame->data = NULL;
  frame->pageNo = -1; // Page isn't assigned
  frame->rBit = UNSET;
  frame->mBit = CLEAN;
}

int secondChance(struct ClockFrame* frame) {
  fprintf(stderr, "secondChance(...) has been called\r\n");
  int rBit = frame->rBit;
  frame->rBit = UNSET;
  return rBit;
}

struct ClockFrame* newClockFrame (int fd) {
  struct ClockFrame* res = (struct ClockFrame*) malloc(sizeof(struct ClockFrame));
  res->next = NULL;
  res->data = NULL;
  res->fd = fd;
  res->pageNo = -1; // Page isn't assigned
  res->rBit = UNSET;
  res->mBit = CLEAN;
  return res;
}

struct Clock* setup_clock(size_t numFrames, int fd) {
  fprintf(stderr, "setup_clock(...) has been called (numFrames: %ld)\r\n", numFrames);
  struct Clock* res = (struct Clock*) malloc(sizeof(struct Clock));
  res->first = NULL;
  res->current = NULL;
  res->numFrames = numFrames;

  struct ClockFrame* previous = NULL;
  struct ClockFrame* curr = NULL;
  for (int i = 0; i < numFrames; ++i) {
      curr = newClockFrame(fd);
      if (previous) {
        previous->next = curr;
      }

      previous = curr;

      if (!res->first) {
        res->first = curr;
        res->current = curr;
      }

      curr->pageNo = i;
      load(curr);
  }

  curr->next = res->first;
  return res;
}

void free_clock(struct Clock* clock) {
  fprintf(stderr, "free_clock(...) has been called\r\n");
  for (struct ClockFrame* frame = clock->first; frame;) {
    struct ClockFrame* curr = frame;
    frame = curr->next;
    if (curr->data) {
      free(curr->data);
      curr->data = NULL;
    }

    free(curr);
    if (frame == clock->first) {
      break;
    }
  }

  clock->first = NULL;
  clock->current = NULL;
  clock->numFrames = 0;
  free(clock);
}

struct ClockFrame *getFrame(struct Clock* clock, size_t pageNo) {
  fprintf(stderr, "getFrame(...) has been called\r\n");
}

void* readData(struct Clock* clock, size_t addr) {
  fprintf(stderr, "readData(...) has been called\r\n");
}

void writeData(struct Clock* clock, size_t addr, void* data, size_t size) {
  fprintf(stderr, "writeData(...) has been called\r\n");
}

void printClock(struct Clock* clock) {
    struct ClockFrame* frame = clock->first;
    assert(frame);

    for (size_t i = 0; i < clock->numFrames; i++) {
        printf("Page %lu with rBit: ", frame->pageNo);
        if (frame->rBit == SET) {
            printf("SET ");
        } else {
            printf("UNSET ");
        }
        printf("and mBit: ");
        if (frame->mBit == CLEAN) {
            printf("CLEAN.\n");
        } else {
            printf("DIRTY.\n");
        }

        frame = frame->next;
    }
}
