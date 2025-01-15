/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   summon_philos.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:26:22 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/15 10:58:18 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	start_watchdog_thread(pthread_t *kthread, t_philo *pp);
static void	cleanup(pthread_t *kthread, sem_t *forks, t_philo *pp);
static void	*killer_thread(void *arg);
static void	philo_subroutine(t_philo *pp, sem_t *forks);

void	summon_philos(int philno, t_philo *pp, sem_t *forks)
{
	int			i;
	int			ppid;
	long int	time0;

	time0 = gettime();
	i = -1;
	while (++i < philno)
	{
		pp->id = i + 1;
		pp->t0 = time0;
		ppid = fork();
		if (ppid < 0)
			return ;
		if (ppid == 0)
			philo_subroutine(pp, forks);
	}
}

static void	philo_subroutine(t_philo *pp, sem_t *forks)
{
	pthread_t	killerthread;
	int long	meal_start;

	start_watchdog_thread(&killerthread, pp);
	while (1)
	{
		eat(pp, forks);
		if (!any_dead(pp) && !pp->status)
		{
			printf("%ld %d is sleeping\n", gettime() - pp->t0, pp->id);
			usleep (pp->time_to_sleep * 1000);
		}
		if (!any_dead(pp) && !pp->status)
		{
			printf("%ld %d is thinking\n", gettime() - pp->t0, pp->id);
			if (pp->philno % 2)
				usleep((pp->time_to_eat * 2 - pp->time_to_sleep) * 1000);
		}
		if (any_dead(pp) || pp->status)
			cleanup(&killerthread, forks, pp);
	}
}

static void	start_watchdog_thread(pthread_t *kthread, t_philo *pp)
{
	if (pthread_create(kthread, NULL, killer_thread, (void *)pp) != 0)
	{
		printf("Thread creation failed");
		exit(1);
	}
}

static void	*killer_thread(void *arg)
{
	t_philo		*ph;
	long int	time;

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
			return (NULL);
		}
		if (any_dead(ph) || ph->status)
			return (NULL);
	}
}

static void	cleanup(pthread_t *kthread, sem_t *forks, t_philo *pp)
{
	pthread_join(*kthread, NULL);
	sem_close(forks);
	sem_close(pp->death);
	sem_close(pp->deathcheck);
	free(pp);
	exit(0);
}
