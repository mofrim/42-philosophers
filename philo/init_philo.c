/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 20:44:24 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/24 10:33:02 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int		alloc_data_structs(t_philo **philos, pthread_mutex_t **forks,
					pthread_t **threads, int phils);
static 	void	init_common_structs(t_philo *ph, pthread_mutex_t *forks,
		pthread_t *threads, pthread_t *clock);
static t_philo	*prep_philos(t_params par);
static void		init_common_params(t_philo	*ph, t_params par, t_clock *clock);

t_philo	*init_philos(t_params par)
{
	int		i;
	t_philo	*phs;

	phs = prep_philos(par);
	i = -1;
	while (++i < par.philno)
	{
		phs[i].id = i + 1;
		if (pthread_mutex_init(&phs->forks[i], NULL) != 0)
			return (printf("Mutex init failed"), NULL);
		if (pthread_create(&(phs->phil_threads)[i], NULL, philo,
			(void *)&phs[i]) != 0)
			return (printf("Thread creation failed"), NULL);
	}
	return (phs);
}

t_philo	*prep_philos(t_params par)
{
	t_philo			*ph;
	t_clock			*clock;
	pthread_t		*clock_thread;
	pthread_t		*threads;
	pthread_mutex_t	*forks;

	if (init_clock(&clock) == -1)
		return (NULL);
	clock_thread = run_the_clock_thread(clock);
	if (alloc_data_structs(&ph, &forks, &threads, par.philno) == -1)
		return (NULL);
	init_common_params(ph, par, clock);
	init_common_structs(ph, forks, threads, clock_thread);
	return (ph);
}

int	alloc_data_structs(t_philo **philos, pthread_mutex_t **forks, \
		pthread_t **threads, int philno)
{
	*philos = malloc(sizeof(t_philo) * philno);
	if (!philos)
		return (-1);
	*forks = malloc(sizeof(pthread_mutex_t) * philno);
	if (!forks)
		return (free(philos), -1);
	*threads = malloc(sizeof(pthread_t) * philno);
	if (!threads)
		return (free(philos), free(forks), -1);
	memset(*threads, 0, philno * sizeof(pthread_t));
	memset(*forks, 0, philno * sizeof(pthread_mutex_t));
	return (0);
}

/* Init the easy stuff. Explanation for status entry: 0 = normal, 1 = fed, 2 =
 * dead. */
void	init_common_params(t_philo	*ph, t_params par, t_clock *clock)
{
	long int	time0;
	int			i;

	time0 = gettime();
	clock->t0 = time0;
	i = -1;
	while (++i < par.philno)
	{
		ph[i].t0 = time0;
		ph[i].last_meal_start = time0;
		ph[i].philno = par.philno;
		ph[i].time_to_die = par.ttd;
		ph[i].time_to_eat = par.tte;
		ph[i].time_to_sleep = par.tts;
		ph[i].num_of_meals = 0;
		ph[i].status = 0;
		ph[i].max_meals = par.maxmeals;
		ph[i].clock = clock;
	}
}

void	init_common_structs(t_philo *ph, pthread_mutex_t *forks,
		pthread_t *threads, pthread_t *clock)
{
	int	i;

	i = -1;
	while (++i < ph[0].philno)
	{
		ph[i].forks = forks;
		ph[i].phil_threads = threads;
		ph[i].clock_thread = clock;
	}
}
