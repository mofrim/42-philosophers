/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_dinner.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:26:22 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/17 22:53:08 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	start_death_thread(pthread_t *kthread, t_philo *pp);
static void	cleanup_and_exit(pthread_t *kthread, t_philo *pp);
static void	*death_thread(void *arg);
static void	philo_subroutine(t_philo *pp);

/* Launch a subprocess for every philo. */
void	philo_dinner(t_philo *pp)
{
	int		i;
	pid_t	cpid;

	i = -1;
	while (++i < pp->philno)
	{
		pp->id = i + 1;
		cpid = fork();
		if (cpid < 0)
			return ;
		if (cpid == 0)
			philo_subroutine(pp);
	}
}

/* The per philo subroutine running in the child process. */
static void	philo_subroutine(t_philo *pp)
{
	pthread_t	killerthread;

	start_death_thread(&killerthread, pp);
	while (1)
	{
		if (!pp->num_of_meals && !(pp->philno % 2) && pp->id % 2)
			usleep((pp->time_to_eat / 2) * 1000);
		philo_eat(pp);
		if (pp->philno != 1 && !any_dead(pp) && !pp->status)
		{
			printf("%ld %d is sleeping\n", gettime() - pp->t0, pp->id);
			usleep (pp->time_to_sleep * 1000);
		}
		if (pp->philno != 1 && !any_dead(pp) && !pp->status)
		{
			printf("%ld %d is thinking\n", gettime() - pp->t0, pp->id);
			if (pp->philno % 2)
				usleep((pp->time_to_eat * 2 - pp->time_to_sleep) * 1000);
		}
		if (any_dead(pp) || pp->status == 1)
			cleanup_and_exit(&killerthread, pp);
	}
}

/* Wrapper for death thread. */
static void	start_death_thread(pthread_t *kthread, t_philo *pp)
{
	if (pthread_create(kthread, NULL, death_thread, (void *)pp) != 0)
	{
		printf("Thread creation failed");
		exit(1);
	}
}

/**
 * The death thread.
 *
 * Kill a philo if it has not eaten often enough. If one philo dies, sem_post
 * enough forks for every other philo to not have them waiting for forks so
 * their processes can terminate cleanly.
 */
static void	*death_thread(void *arg)
{
	t_philo		*ph;
	long int	time;
	int			i;

	ph = (t_philo *)arg;
	while (1)
	{
		usleep(9000);
		time = gettime() - ph->t0;
		if (time - ph->last_meal_start >= ph->time_to_die)
		{
			printf("%ld %d died\n", time, ph->id);
			ph->status = 2;
			sem_post(ph->death);
			i = -1;
			while (++i < 2 * ph->philno)
				sem_post(ph->forks);
			return (NULL);
		}
		if (any_dead(ph) || ph->status)
			return (NULL);
	}
}

static void	cleanup_and_exit(pthread_t *kthread, t_philo *pp)
{
	pthread_join(*kthread, NULL);
	sem_close(pp->forks);
	sem_close(pp->death);
	sem_close(pp->deathcheck);
	sem_close(pp->fed);
	sem_close(pp->fedcheck);
	free(pp->semas);
	free(pp);
	exit(0);
}
