#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PAGE_SIZE 4096

enum R_BIT { UNSET, SET };
enum M_BIT { CLEAN, DIRTY };

struct ClockFrame
{
	struct ClockFrame *next;
	void *data;
	int fd;
	size_t pageNo;
	enum R_BIT rBit;
	enum M_BIT mBit;
};

struct Clock
{
	struct ClockFrame *first;
	struct ClockFrame *current;
	size_t numFrames;
};

size_t getPageNo(size_t addr);

size_t getOffset(size_t addr);

void load(struct ClockFrame *frame);

void store(struct ClockFrame *frame);

int secondChance(struct ClockFrame *frame);

struct Clock *setup_clock(size_t numFrames, int fd);

void free_clock(struct Clock *clock);

struct ClockFrame *getFrame(struct Clock *clock, size_t pageNo);

void *readData(struct Clock *clock, size_t addr);

void writeData(struct Clock *clock, size_t addr, void *data, size_t size);

void printClock(struct Clock *clock);