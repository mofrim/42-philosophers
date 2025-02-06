/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   order_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 13:00:46 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/06 13:03:28 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Quick and dirty order of timestamps checker. Normi wouldn't like this 8-P */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

int main(int ac, char **av)
{
	FILE *f;
	char	*w;
	char	*num1, *num2;

	(void)ac;
	num1 = calloc(100, 1);
	num2 = calloc(100, 1);
	f = fopen(av[1], "r");
	if (!f)
		exit(1);
	char line1[256];
	char line2[256];
	if (fgets(line1, 255, f) == NULL)
		exit(1);
	while(fgets(line2, 255, f) != NULL)
	{
		w = strchr(line1, ' ');
		if (w)
		{
			bzero(num1, 100);
			strncpy(num1, line1, w - line1);
		}
		w = strchr(line2, ' ');
		if (w)
		{
			bzero(num2, 100);
			strncpy(num2, line2, w - line2);
		}
		if (isdigit(*num2) && atoi(num1) > atoi(num2))
		{
			printf("num1 = %s > num2 = %s\n", num1, num2);
			printf("line1 = %s", line1);
			printf("line2 = %s\n", line2);
		}
		bzero(line1, 256);
		strcpy(line1, line2);
	}
	free(num1);
	free(num2);
	fclose(f);
	return (0);
}
