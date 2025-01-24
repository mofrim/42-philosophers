#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int ac, char **av)
{
	FILE *f;
	char	*w;
	char	*num1, *num2;

	num1 = calloc(100, 1);
	num2 = calloc(100, 1);
	f = fopen(av[1], "r");
	if (!f)
		exit(1);
	char line[256];
	while(fgets(line, 255, f) != NULL)
	{
		w = strchr(line, ' ');
		if (w)
			strncpy(num1, line, w - line);
		if (fgets(line, 255, f) != NULL)
		{
			w = strchr(line, ' ');
			if (w)
				strncpy(num2, line, w - line);
		}
		if (atoi(num1) > atoi(num2))
			printf("num1 = %s > num2 = %s\n", num1, num2);
	}
	return (0);
}
