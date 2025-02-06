/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 20:42:36 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/06 12:20:00 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ph_eat(t_philo *ph, int id);
static void	ph_sleep(t_philo *ph, int id);
static void	ph_think(t_philo *ph, int id);

/** The main philosopher thread routine.
 *
 * The transfer of the id is a bit of a hack. But otherwise i would have gotten
 * into problems with my single philo struct approach. The only purpose of the
 * ph->init_lock mutex this is.
 */
void	*philo(void *phv)
{
	t_philo	*ph;
	int		id;

	ph = (t_philo *)phv;
	id = ph->id;
	pthread_mutex_unlock(ph->init_lock);
	while (1)
	{
		if (!ph->num_of_meals[id - 1] && (id % 2))
			usleep((ph->time_to_eat / 2) * 1000);
		if (ph->status[id - 1] != 0)
			return (NULL);
		ph_eat(ph, id);
		ph_sleep(ph, id);
		ph_think(ph, id);
	}
}

/* Waiting for the fork function. Try to get a mutex_lock on one fork. The
 * little bit smelly `first` flag is a side-effect of this abstraction.
 * Otherwise we will not be able to pick right/left fork using this function.
 */
static void	wait_for_fork(t_philo *ph, int *gotfork, int id, int first)
{
	if (first)
		pthread_mutex_lock(&ph->forks[id - 1]);
	else
		pthread_mutex_lock(&ph->forks[id % ph->philno]);
	(*gotfork)++;
	if (!any_dead(ph))
		print_logmsg(id, "has taken a fork", ph);
}

/**
 * The eating function of philo.
 *
 * This is the most complex part of a philos life.
 *
 * To avoid corrupting mutex state by mutliple mutex_unlocks, i intruduced the
 * 'gotfork' flag. This ensures even in case we enter this function in a already
 * dead state (which could happen if philo dies during initial sleep) that
 * mutex_unlock is not called on a not locked mutex. This could lead to errors
 * later especially mutex_destroy reports errno 16 (EBUSY) with mutexes like
 * this.
 */
void	ph_eat(t_philo *ph, int id)
{
	int		gotfork;

	gotfork = 0;
	if (!any_dead(ph))
		wait_for_fork(ph, &gotfork, id, 1);
	if (ph->philno != 1 && !any_dead(ph))
		wait_for_fork(ph, &gotfork, id, 0);
	if (ph->philno != 1 && !ph->status[id - 1] && !any_dead(ph) && gotfork == 2)
	{
		print_logmsg(id, "is eating", ph);
		ph->last_meal_start[id - 1] = gettime();
		usleep (ph->time_to_eat * 1000);
		ph->num_of_meals[id - 1]++;
		if (ph->max_meals && ph->num_of_meals[id - 1] == ph->max_meals)
			ph->status[id - 1] = 1;
	}
	if (ph->philno != 1 && gotfork)
		pthread_mutex_unlock(&ph->forks[id - 1]);
	if (gotfork == 2)
		pthread_mutex_unlock(&ph->forks[id % ph->philno]);
}

/* Sleeping function nothing interesting happening here. */
void	ph_sleep(t_philo *ph, int id)
{
	if (ph->philno != 1 && !ph->status[id - 1] && !any_dead(ph))
	{
		print_logmsg(id, "is sleeping", ph);
		usleep(ph->time_to_sleep * 1000);
	}
}

/* Thinking function. The thinking syncronization is not really needed for
 * manda, because mutexes_lock seems to almost certainly work with the FIFO
 * principle. For explanation see my PDF. */
void	ph_think(t_philo *ph, int id)
{
	if (ph->philno != 1 && !ph->status[id - 1] && !any_dead(ph))
	{
		print_logmsg(id, "is thinking", ph);
		if (ph->philno % 2 && (ph->time_to_sleep < 2 * ph->time_to_eat))
			usleep((ph->time_to_eat * 2 - ph->time_to_sleep) * 1000);
	}
}
