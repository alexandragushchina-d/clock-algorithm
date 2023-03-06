#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{

	int file, x;
	file = open("storage.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

	x = 0;

	for(int i=0; i<262144; i++)
	{
		pwrite(file, &x, sizeof(int), i*sizeof(int));
	}

	close(file);
	return EXIT_SUCCESS;
}
