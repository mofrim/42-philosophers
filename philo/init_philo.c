/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 20:44:24 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/20 22:25:04 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_common_params(t_philo	*ph, char **av, int id);
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

t_philo	*init_philos(char **av, int ac)
{
	pthread_mutex_t	*forks;
	t_philo			*philos;
	pthread_t		*threads;
	int				i;
	int				phils;

	phils = ft_atoi(av[1]);
	if (check_invalid_params(av, ac))
		return (NULL);
	if (!alloc_data_structures(&philos, &forks, &threads, phils))
		return (NULL);
	i = -1;
	while (++i < phils)
	{
		init_common_params(&philos[i], av, i + 1);
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
		pthread_t **threads, int phils)
{
	*philos = malloc(sizeof(t_philo) * phils);
	if (!philos)
		return (0);
	*forks = malloc(sizeof(pthread_mutex_t) * phils);
	if (!forks)
		return (free(philos), 0);
	*threads = malloc(sizeof(pthread_t) * phils);
	if (!threads)
		return (free(philos), free(forks), 0);
	memset(*threads, 0, phils * sizeof(pthread_t));
	memset(*forks, 0, phils * sizeof(pthread_mutex_t));
	return (1);
}

/* Init the easy stuff. Explanation for status entry: 0 = normal, 1 = fed, 2 =
 * dead. */
void	init_common_params(t_philo	*ph, char **av, int id)
{
	long int	time0;

	time0 = gettime();
	ph->id = id;
	ph->t0 = time0;
	ph->last_meal_start = time0;
	ph->num_of_philos = ft_atoi(av[1]);
	ph->time_to_die = ft_atoi(av[2]);
	ph->time_to_eat = ft_atoi(av[3]);
	ph->time_to_sleep = ft_atoi(av[4]);
	ph->num_of_meals = 0;
	ph->status = 0;
	ph->max_meals = 0;
}
