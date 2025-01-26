/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 20:42:36 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/25 14:12:55 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ph_eat(t_philo *ph, int id);
static void	ph_sleep(t_philo *ph, int id);
static void	ph_think(t_philo *ph, int id);

/* The main philosopher thread routine. */
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
		if (ph->status[id - 1] == 2)
			return (NULL);
		ph_eat(ph, id);
		ph_sleep(ph, id);
		ph_think(ph, id);
	}
}

/**
 * The eating function of philo.
 *
 * To avoid corrupting mutex state by mutliple mutex_unlocks, i intruduced the
 * 'locked' flag. This ensures even in case we enter this function in a already
 * dead state (which could happen if philo dies during initial sleep) that
 * mutex_unlock is not called on a not locked mutex. This could lead to errors
 * later especially mutex_destroy reports errno 16 (EBUSY) with mutexes like
 * this.
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

/* Thinking function. */
void	ph_think(t_philo *ph, int id)
{
	if (ph->philno != 1 && !ph->status[id - 1] && !any_dead(ph))
	{
		print_logmsg(ph->id, "is thinking", ph);
		if (ph->philno % 2)
			usleep((ph->time_to_eat * 2 - ph->time_to_sleep) * 1000);
	}
}
