/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 11:01:54 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/15 12:25:00 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	init_common_params(t_philo *ph, int ac, char **av, long int time0)
{
	ph->t0 = time0;
	ph->philno = ft_atoi(av[1]);
	ph->last_meal_start = time0;
	ph->time_to_die = ft_atoi(av[2]);
	ph->time_to_eat = ft_atoi(av[3]);
	ph->time_to_sleep = ft_atoi(av[4]);
	ph->num_of_meals = 0;
	ph->status = 0;
	if (ac == 6)
		ph->max_meals = ft_atoi(av[5]);
	else
		ph->max_meals = 0;
}

t_philo	*parse_philo(int ac, char **av, sem_t *death, sem_t *deathcheck)
{
	t_philo		*ph;
	int			i;
	long int	time0;
	int			philno;

	philno = ft_atoi(av[1]);
	ph = malloc(sizeof(t_philo));
	i = -1;
	time0 = gettime();
	ph->id = 1;
	ph->death = death;
	ph->deathcheck = deathcheck;
	init_common_params(ph, ac, av, time0);
	return (ph);
}
