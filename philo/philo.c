/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 11:53:00 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/13 07:33:28 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO: refactor! normify!

#include "philo.h"
#include <stdio.h>
#include <string.h>

t_philo	*init_philos(char **av, int argnum);
void	philo(void *phv); int	all_are_dead(t_philo *ph);
int		any_dead(t_philo *ph);
int		all_are_fed(t_philo *ph);

int	main(int ac, char **av)
{
	t_philo	*philos;
	int		num_of_philos;
	int		ttd;


	if (5 <= ac && ac <= 6)
		philos = init_philos(av, ac);
	else
		printf("usage: %s num_of_philos ttd tte tts (numoftimes_to_eat)\n", av[0]);
	while (1)
	{
		usleep(10000); // sleep 10ms
		if (any_dead(philos))
			exit(42);
		if (all_are_dead(philos) || all_are_fed(philos))
			return (1);
	}
	for (int i = 0; i < num_of_philos; i++)
	{
		pthread_join(philos[i].phil_threads[i], NULL);
		pthread_mutex_destroy(&philos[i].forks[i]);
	}
	return (0);
}

int	all_are_dead(t_philo *ph)
{
	int	i;

	i = -1;
	while (++i < ph[0].num_of_philos)
		if (ph[i].status != 2)
			return (0);
	return (1);
}

int	all_are_fed(t_philo *ph)
{
	int	i;

	i = -1;
	while (++i < ph[0].num_of_philos)
		if (ph[i].status != 1)
			return (0);
	return (1);
}

int	any_dead(t_philo *ph)
{
	int		i;
	long	time;

	i = -1;
	time = gettime();
	while (++i < ph[0].num_of_philos)
	{
		if (time - ph[i].last_meal_start > ph[0].time_to_die && \
				ph[i].status == 0)
		{
			ph[i].status = 2;
			printf("%ld %d died\n", time - ph[i].t0, ph[i].id);
			return (1);
		}
	}
	return (0);
}

t_philo	*init_philos(char **av, int argnum)
{
	pthread_mutex_t	*forks;
	t_philo			*philos;
	pthread_t		*threads;
	int				phils;
	int				i;
	long int		time0;


	phils = ft_atoi(av[1]);
	if (1 > phils || phils > 10000)
	{
		printf("Too many or to little philos!\n");
		exit(1);
	}
	philos = malloc(sizeof(t_philo) * phils);
	if (!philos)
		exit(EXIT_FAILURE);
	forks = malloc(sizeof(pthread_mutex_t) * phils);
	if (!forks)
		exit(EXIT_FAILURE);
	threads = malloc(sizeof(pthread_t) * phils);
	if (!threads)
		exit(EXIT_FAILURE);
	memset(threads, 0, phils * sizeof(pthread_t));
	memset(forks, 0, phils * sizeof(pthread_mutex_t));

	time0 = gettime();
	i = -1;
	while (++i < phils)
	{
		philos[i].id = i + 1;
		philos[i].t0 = time0;
		philos[i].last_meal_start = time0;
		philos[i].num_of_philos = phils;
		philos[i].time_to_die = ft_atoi(av[2]);
		if (philos[i].time_to_die <= 0)
		{
			printf("Time to die invalid.\n");
			exit(42);
		}
		philos[i].time_to_eat = ft_atoi(av[3]);
		if (philos[i].time_to_eat <= 0)
		{
			printf("Time to eat invalid.\n");
			exit(42);
		}
		philos[i].time_to_sleep = ft_atoi(av[4]);
		if (philos[i].time_to_sleep <= 0)
		{
			printf("Time to sleep invalid.\n");
			exit(42);
		}
		philos[i].num_of_meals = 0;
		if (argnum == 6)
		{
			philos[i].max_meals = ft_atoi(av[5]);
			if (philos[i].max_meals <= 0)
			{
				printf("max_meals invalid.\n");
				exit(42);
			}
		}
		philos[i].status = 0; // 0 = normal, 1 = done, 2 = dead
		philos[i].forks = forks;
		philos[i].phil_threads = threads;
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			printf("Mutex init failed\n");
			exit(EXIT_FAILURE);
		}
		if (pthread_create(&threads[i], NULL, (void *)philo, (void *)&philos[i]) != 0)
		{
			printf("Thread creation failed\n");
			exit(EXIT_FAILURE);
		}
	}
	return (philos);
}

void	philo(void *phv)
{
	t_philo	*ph;

	ph = (t_philo *)phv;
	while (1)
	{
		if (ph->status == 2)
			return ;
		if (!ph->num_of_meals)
			if (!(ph->id % 2))
				usleep((ph->time_to_eat/2) * 1000);
		if (ph->num_of_meals && ph->num_of_meals == ph->max_meals)
		{
			ph->status = 1;
			return ;
		}

		/* eat */
		pthread_mutex_lock(&ph->forks[ph->id - 1]);
		printf("%ld %d has taken a fork\n", gettime() - ph->t0 , ph->id);
		if (ph->num_of_philos != 1)
		{
			pthread_mutex_lock(&ph->forks[(ph->id) % ph->num_of_philos]);
			printf("%ld %d has taken a fork\n", gettime() - ph->t0 , ph->id);
			ph->last_meal_start = gettime();
			printf("%ld %d is eating\n",ph->last_meal_start - ph->t0, ph->id);
			usleep(ph->time_to_eat * 1000);
			pthread_mutex_unlock(&ph->forks[(ph->id) % ph->num_of_philos]);
			ph->num_of_meals++;
		}
		pthread_mutex_unlock(&ph->forks[ph->id - 1]);

		/* sleep */
		printf("%ld %d is sleeping\n", gettime() - ph->t0, ph->id);
		usleep(ph->time_to_sleep * 1000);

		/* think */
		printf("%ld %d is thinking\n", gettime() - ph->t0, ph->id);
	}
}

