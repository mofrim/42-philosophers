/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 20:44:24 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/23 13:49:19 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_common_params(t_philo	*ph, char **av, int philno);
int			alloc_data_structures(t_philo **philos, pthread_mutex_t **forks, \
				pthread_t **threads, int phils);

int	check_invalid_params(char **av, int ac)
{
	int	phils;

	phils = ft_atoi(av[1]);
	if (1 > phils || phils > 1000000)
		return (1);
	if (ft_atoi(av[2]) <= 0)
		return (1);
	if (ft_atoi(av[3]) <= 0)
		return (1);
	if (ft_atoi(av[4]) <= 0)
		return (1);
	if (ac == 6)
		if (ft_atoi(av[5]) <= 0)
			return (1);
	return (0);
}

t_philo	*init_philos(char **av, int ac, int philno)
{
	pthread_mutex_t	*forks;
	t_philo			*philos;
	pthread_t		*threads;
	int				i;

	if (check_invalid_params(av, ac))
		return (NULL);
	if (!alloc_data_structures(&philos, &forks, &threads, philno))
		return (NULL);
	init_common_params(philos, av, philno);
	i = -1;
	while (++i < philno)
	{
		philos[i].id = i + 1;
		if (ac == 6)
			philos[i].max_meals = ft_atoi(av[5]);
		philos[i].forks = forks;
		philos[i].phil_threads = threads;
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (printf("Mutex init failed"), NULL);
		if (pthread_create(&threads[i], NULL, philo, (void *)&philos[i]) != 0)
			return (printf("Thread creation failed"), NULL);
	}
	return (philos);
}

int	alloc_data_structures(t_philo **philos, pthread_mutex_t **forks, \
		pthread_t **threads, int philno)
{
	*philos = malloc(sizeof(t_philo) * philno);
	if (!philos)
		return (0);
	*forks = malloc(sizeof(pthread_mutex_t) * philno);
	if (!forks)
		return (free(philos), 0);
	*threads = malloc(sizeof(pthread_t) * philno);
	if (!threads)
		return (free(philos), free(forks), 0);
	memset(*threads, 0, philno * sizeof(pthread_t));
	memset(*forks, 0, philno * sizeof(pthread_mutex_t));
	return (1);
}

/* Init the easy stuff. Explanation for status entry: 0 = normal, 1 = fed, 2 =
 * dead. */
void	init_common_params(t_philo	*ph, char **av, int philno)
{
	long int	time0;
	int			i;

	time0 = gettime();
	i = -1;
	while (++i < philno)
	{
		ph[i].t0 = time0;
		ph[i].last_meal_start = time0;
		ph[i].num_of_philos = ft_atoi(av[1]);
		ph[i].time_to_die = ft_atoi(av[2]);
		ph[i].time_to_eat = ft_atoi(av[3]);
		ph[i].time_to_sleep = ft_atoi(av[4]);
		ph[i].num_of_meals = 0;
		ph[i].status = 0;
		ph[i].max_meals = 0;
	}
}
