/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:03:17 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/12 21:55:49 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "sys/time.h"

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

void	print_philo_data(t_philo p)
{
	printf("p.id = %d\np.t0 = %ld\np.last_meal_start = %ld\np.num_of_philos = %d\np.time_to_die = %d\np.time_to_eat = %d\np.time_to_sleep = %d\n\np.num_of_meals = %d\np.max_meals = %d\np.status = %d\n", p.id, p.t0, p.last_meal_start, p.num_of_philos, p.time_to_die, p.time_to_eat, p.time_to_sleep, p.num_of_meals, p.max_meals, p.status);
}
