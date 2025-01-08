/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:03:17 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/08 08:37:05 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isspace(char c)
{
	return (c == '\f' || c == '\n' || c == '\r' || c == '\t'
		|| c == ' ' || c == '\v');
}

static int	ft_isdigit(int c)
{
	if (48 <= c && c <= 57)
		return (2048);
	return (0);
}

int	ft_atoi(char *s)
{
	int	num;
	int	sgn;

	num = 0;
	sgn = 1;
	while (ft_isspace(*s))
		s++;
	if (*s == '-')
		sgn = -1;
	if (*s == '-' || *s == '+')
		s++;
	while (ft_isdigit(*s))
		num = num * 10 + *s++ - '0';
	return (num * sgn);
}

/* Get current time in milliseconds since epoch. */
long int	gettime(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
