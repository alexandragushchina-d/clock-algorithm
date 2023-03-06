## Clock (the second chance) page replacement algorithm


This task deals with the implementation of the clock algorithm for page replacement. The aim of this algorithm is to manage the main memory using a second-chance strategy for page swapping in the event of a page fault. 

In order to simulate the swapping in and out of pages from background memory, a file is used thata file representing the background memory is used to simulate the swapping in and out of pages from the background memory. Reading the data into an allocated memory area and writing it back corresponds to swapping in and out. These additional memory areas represent the main memory and are referenced by so-called "clock frames", which represent the individual tiles of the clock. 

At the beginning of an execution scenario, a clock consisting of several clock frames is created. The algorithm is then executed on this clock.
