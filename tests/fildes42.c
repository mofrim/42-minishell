#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// write a program that prints to stdout, stderr and fildes number 42.
int main(int ac, char **av)
{
	// int fd = open("42.42", O_RDWR | O_CREAT | O_APPEND, 0600);
	// dup2(fd, 42);
	// close(fd);
	dprintf(1, "This is my msg to stdout!\n");
	dprintf(2, "This is my msg to stderr!\n");
	dprintf(42, "This is my msg to 42!\n");
	return (0);
}
