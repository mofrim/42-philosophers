/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 20:42:36 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/24 12:12:02 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ph_initial_sleep(t_philo *ph, int id);
static void	ph_eat(t_philo *ph, int id);
static void	ph_sleep(t_philo *ph, int id);
static void	ph_think(t_philo *ph, int id);

/* The main philosopher thread routine. */
void	*philo(void *phv)
{
	t_philo	*ph;
	int		id;

	ph = (t_philo *)phv;
	id = *ph->id;
	printf("id = %d\n", id);
	while (1)
	{
		if (ph->status == 2)
			return (NULL);
		ph_initial_sleep(ph, id);
		if (ph->num_of_meals && ph->num_of_meals == ph->max_meals)
		{
			ph->status = 1;
			return (NULL);
		}
		ph_eat(ph, id);
		ph_sleep(ph, id);
		ph_think(ph, id);
	}
}

/* Well this is my simple synchronization. Make every 2nd philosopher sleep half
 * of eating time to avoid deadlock from the start. */
void	ph_initial_sleep(t_philo *ph, int id)
{
	if (!ph->num_of_meals && (id % 2))
		usleep((ph->time_to_eat / 2) * 1000);
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
void	ph_eat(t_philo *ph, int id)
{
	int	locked;

	locked = 0;
	if (!ph->status)
	{
		locked = !(pthread_mutex_lock(&ph->forks[id - 1]));
		if (!ph->status)
			printf("%ld %d has taken a fork\n", gettime() - ph->t0, id);
	}
	if (ph->philno != 1 && !ph->status)
	{
		pthread_mutex_lock(&ph->forks[id % ph->philno]);
		if (!ph->status)
		{
			printf("%ld %d has taken a fork\n", gettime() - ph->t0, id);
			ph->last_meal_start = gettime();
			printf("%ld %d is eating\n", ph->last_meal_start - ph->t0, id);
			usleep(ph->time_to_eat * 1000);
			ph->num_of_meals++;
		}
		pthread_mutex_unlock(&ph->forks[id % ph->philno]);
	}
	if (locked)
		pthread_mutex_unlock(&ph->forks[id - 1]);
}

/* Sleeping function nothing interesting happening here. */
void	ph_sleep(t_philo *ph, int id)
{
	if (!ph->status)
	{
		printf("%ld %d is sleeping\n", gettime() - ph->t0, id);
		usleep(ph->time_to_sleep * 1000);
	}
}

/* Thinking function. */
void	ph_think(t_philo *ph, int id)
{
	if (!ph->status)
	{
		printf("%ld %d is thinking\n", gettime() - ph->t0, id);
		if (ph->philno % 2)
			usleep((ph->time_to_eat * 2 - ph->time_to_sleep) * 1000);
	}
}
