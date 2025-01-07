#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "../libft/libft.h"

// write a program that prints to stdout, stderr and fildes number 42.
int main(int ac, char **av)
{
	char	*str;

	(void)av;
	(void)ac;
	dprintf(1, "This is my msg to stdout!\n");
	dprintf(2, "This is my msg to stderr!\n");
	dprintf(42, "This is my msg to 42!\n");
	str = get_next_line(23);
	printf("from fd 23: %s\n", str);
	while (str)
	{
		free(str);
		str = get_next_line(23);
	}
	return (0);
}
