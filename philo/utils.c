/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:03:17 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/23 15:40:58 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	while (*s == '\f' || *s == '\n' || *s == '\r' || *s == '\t'
		|| *s == ' ' || *s == '\v')
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
long	gettime(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	check_invalid_params(char **av, int ac)
{
	int	philno;

	philno = ft_atoi(av[1]);
	if (1 > philno || philno > 1000000)
		return (-1);
	if (ft_atoi(av[2]) <= 0)
		return (-1);
	if (ft_atoi(av[3]) <= 0)
		return (-1);
	if (ft_atoi(av[4]) <= 0)
		return (-1);
	if (ac == 6)
		if (ft_atoi(av[5]) <= 0)
			return (-1);
	return (0);
}

t_params	*parse_params(int ac, char **av)
{
	t_params	*params;

	if (check_invalid_params(av, ac) == -1)
		return (NULL);
	params = malloc(sizeof(t_params));
	if (params == NULL)
		return (NULL);
	params->philno = ft_atoi(av[1]);
	params->ttd = ft_atoi(av[2]);
	params->tte = ft_atoi(av[3]);
	params->tts = ft_atoi(av[4]);
	params->maxmeals = 0;
	if (ac == 6)
		params->maxmeals = ft_atoi(av[5]);
	return (params);
}
