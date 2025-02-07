/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:35:37 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/07 15:43:19 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * The death thread.
 *
 * Kill a philo if it has not eaten often enough. If one philo dies, sem_post
 * enough forks for every other philo to not have them waiting for forks so
 * their processes can terminate cleanly.
 */
void	*death_thread(void *arg)
{
	t_philo		*ph;
	long int	time;
	int			i;

	ph = (t_philo *)arg;
	while (1)
	{
		usleep(9000);
		time = gettime() - ph->t0;
		sem_wait(ph->state);
		if (time - ph->last_meal_start >= ph->time_to_die)
		{
			sem_post(ph->state);
			print_logmsg("died", ph);
			ph->status = 2;
			sem_post(ph->death);
			i = -1;
			while (++i < 2 * ph->philno)
				sem_post(ph->forks);
			return (sem_post(ph->state), NULL);
		}
		if (any_dead(ph) || ph->status)
			return (sem_post(ph->state), NULL);
		sem_post(ph->state);
	}
}

/* Wrapper for death thread. */
void	start_death_thread(pthread_t *kthread, t_philo *pp)
{
	if (pthread_create(kthread, NULL, death_thread, (void *)pp) != 0)
	{
		printf("Thread creation failed with error: %d\n", errno);
		exit(1);
	}
}
