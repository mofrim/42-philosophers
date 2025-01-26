/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 20:44:24 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/25 14:12:09 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void		init_common_params(t_philo	*ph, t_params par);
static int		alloc_data_structs(t_philo **ph, int philno);
static t_philo	*prep_philo(t_params par);
static int		alloc_init_mutexes(t_philo *ph, int philno);

/** 
 * The main init func. 
 *
 * 1st, prepare the master t_philo struct.
 * 2ndly, loop over the philo id numbers. This still feels a bit hacky but it
 * works. Need the `init_lock` mutex in order to transfer the philo id to its
 * thread. Inside the philo_thread `id` is assigned to a stack variable. After
 * that the mutex is unlocked again so the loop in `init_philos` can set the
 * phs->id var to the next id.
 */
t_philo	*init_philos(t_params par)
{
	int		i;
	t_philo	*phs;

	phs = prep_philo(par);
	i = -1;
	while (++i < par.philno)
	{
		pthread_mutex_lock(phs->init_lock);
		phs->id = i + 1;
		if (pthread_create(&(phs->phil_threads)[i], NULL, philo,
			(void *)phs) != 0)
			return (printf("Thread creation failed"), NULL);
	}
	return (phs);
}

/* Prepper di prep the philo struct. */
t_philo	*prep_philo(t_params par)
{
	t_philo			*ph;

	if (alloc_data_structs(&ph, par.philno) == -1)
		return (NULL);
	if (alloc_init_mutexes(ph, par.philno) == -1)
		return (NULL);
	init_common_params(ph, par);
	return (ph);
}

/* Allocations for everything that is not mutex. */
int	alloc_data_structs(t_philo **ph, int philno)
{
	*ph = malloc(sizeof(t_philo));
	if (!*ph)
		return (-1);
	(*ph)->phil_threads = malloc(sizeof(pthread_t) * philno);
	if (!(*ph)->phil_threads)
		return (free(*ph), -1);
	(*ph)->last_meal_start = malloc(sizeof(long) * philno);
	if (!(*ph)->last_meal_start)
		return (-1);
	(*ph)->num_of_meals = malloc(sizeof(int) * philno);
	if (!(*ph)->num_of_meals)
		return (-1);
	(*ph)->status = malloc(sizeof(int) * philno);
	if (!(*ph)->status)
		return (-1);
	return (0);
}

/* Mutex init and alloc. */
int	alloc_init_mutexes(t_philo *ph, int philno)
{
	int	i;

	ph->init_lock = malloc(sizeof(pthread_mutex_t));
	if (!ph->init_lock)
		return (-1);
	if (pthread_mutex_init(ph->init_lock, NULL) != 0)
		return (free(ph->init_lock), -1);
	ph->print_lock = malloc(sizeof(pthread_mutex_t));
	if (!ph->print_lock)
		return (-1);
	if (pthread_mutex_init(ph->print_lock, NULL) != 0)
		return (free(ph->init_lock), free(ph->print_lock), -1);
	ph->forks = malloc(sizeof(pthread_mutex_t) * philno);
	if (!ph->forks)
		return (free(ph->init_lock), -1);
	i = -1;
	while (++i < philno)
		if (pthread_mutex_init(&ph->forks[i], NULL) != 0)
			return (free(ph->init_lock), free(ph->forks),
				printf("Mutex init failed"), -1);
	return (0);
}

/* Init the easy stuff. Explanation for status entry: 0 = normal, 1 = fed, 2 =
 * dead. */
void	init_common_params(t_philo	*ph, t_params par)
{
	long int	time0;
	int			i;

	time0 = gettime();
	ph->t0 = time0;
	ph->id = 0;
	ph->philno = par.philno;
	ph->time_to_die = par.ttd;
	ph->time_to_eat = par.tte;
	ph->time_to_sleep = par.tts;
	ph->max_meals = par.maxmeals;
	i = -1;
	while (++i < ph->philno)
	{
		ph->last_meal_start[i] = time0;
		ph->num_of_meals[i] = 0;
		ph->status[i] = 0;
	}
}
