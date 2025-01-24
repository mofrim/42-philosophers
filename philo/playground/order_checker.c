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
	char line1[256];
	char line2[256];
	while(fgets(line1, 255, f) != NULL)
	{
		w = strchr(line1, ' ');
		if (w)
			strncpy(num1, line1, w - line1);
		if (fgets(line2, 255, f) != NULL)
		{
			w = strchr(line2, ' ');
			if (w)
				strncpy(num2, line2, w - line2);
		}
		if (atoi(num1) > atoi(num2))
		{
			printf("num1 = %s > num2 = %s\n", num1, num2);
			printf("line1 = %s", line1);
			printf("line2 = %s\n", line2);
		}
	}
	return (0);
}
