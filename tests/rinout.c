// Test program for the `<>` redirection

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "../libft/libft.h"

int main(int ac, char **av)
{
	char	*str;

	(void)av;
	(void)ac;
	str = get_next_line(0);
	printf("this is what i read from fd 0: %s\n", str);
	dprintf(0, "and this is what i also wanted to say.\n");
	while (str)
	{
		free(str);
		str = get_next_line(0);
	}
	return (0);
}
