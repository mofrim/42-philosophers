/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:03:17 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/07 16:14:46 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* All-in-one atoi, isspace & isdigit. */
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
	while (48 <= *s && *s <= 57)
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

t_bool	check_invalid_params(char **av, int ac)
{
	int	phils;

	phils = ft_atoi(av[1]);
	if (1 > phils || phils > 100000)
		return (TRUE);
	if (ft_atoi(av[2]) <= 0)
		return (TRUE);
	if (ft_atoi(av[3]) <= 0)
		return (TRUE);
	if (ft_atoi(av[4]) <= 0)
		return (TRUE);
	if (ac == 6)
		if (ft_atoi(av[5]) <= 0)
			return (TRUE);
	return (FALSE);
}

int	print_logmsg(char *msg, t_philo *ph)
{
	long	timestamp;

	sem_wait(ph->print);
	timestamp = gettime() - ph->t0;
	printf("%ld %d %s\n", timestamp, ph->id, msg);
	sem_post(ph->print);
	return (1);
}
