/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_dinner.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:26:22 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/07 16:06:53 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	cleanup_and_exit(pthread_t *kthread, t_philo *pp);
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
			print_logmsg("is sleeping", pp);
			usleep (pp->time_to_sleep * 1000);
		}
		if (pp->philno != 1 && !any_dead(pp) && !pp->status)
		{
			print_logmsg("is thinking", pp);
			if (pp->philno % 2 && (pp->time_to_sleep < 2 * pp->time_to_eat))
				usleep((pp->time_to_eat * 2 - pp->time_to_sleep) * 1000);
		}
		if (any_dead(pp) || pp->status == 1)
			cleanup_and_exit(&killerthread, pp);
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
	sem_close(pp->print);
	sem_close(pp->state);
	free(pp->semas);
	free(pp);
	exit(0);
}
