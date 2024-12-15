/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 20:42:36 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/15 20:46:40 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ph_initial_sleep(t_philo *ph);
void	ph_eat(t_philo *ph);
void	ph_sleep(t_philo *ph);

void	*philo(void *phv)
{
	t_philo	*ph;

	ph = (t_philo *)phv;
	while (1)
	{
		if (ph->status == 2)
			return (NULL);
		ph_initial_sleep(ph);
		if (ph->num_of_meals && ph->num_of_meals == ph->max_meals)
		{
			ph->status = 1;
			return (NULL);
		}
		ph_eat(ph);
		ph_sleep(ph);
		if (!ph->status)
			printf("%ld %d is thinking\n", gettime() - ph->t0, ph->id);
	}
}

void	ph_initial_sleep(t_philo *ph)
{
	if (!ph->num_of_meals)
	{
		if (!(ph->num_of_philos % 2) && !(ph->id % 2))
			usleep((ph->time_to_eat / 2) * 1000);
		if ((ph->num_of_philos % 2) && (ph->id % 2))
			usleep((ph->time_to_eat / 2) * 1000);
	}
}

void	ph_eat(t_philo *ph)
{
	if (!ph->status)
	{
		pthread_mutex_lock(&ph->forks[ph->id - 1]);
		if (!ph->status)
			printf("%ld %d has taken a fork\n", gettime() - ph->t0, ph->id);
		else
			pthread_mutex_unlock(&ph->forks[ph->id - 1]);
	}
	if (ph->num_of_philos != 1 && !ph->status)
	{
		pthread_mutex_lock(&ph->forks[(ph->id) % ph->num_of_philos]);
		if (!ph->status)
		{
			printf("%ld %d has taken a fork\n", gettime() - ph->t0, ph->id);
			ph->last_meal_start = gettime();
			printf("%ld %d is eating\n", ph->last_meal_start - ph->t0, ph->id);
			usleep(ph->time_to_eat * 1000);
			pthread_mutex_unlock(&ph->forks[(ph->id) % ph->num_of_philos]);
			ph->num_of_meals++;
		}
		else
			pthread_mutex_unlock(&ph->forks[(ph->id) % ph->num_of_philos]);
	}
	pthread_mutex_unlock(&ph->forks[ph->id - 1]);
}

void	ph_sleep(t_philo *ph)
{
	if (!ph->status)
	{
		printf("%ld %d is sleeping\n", gettime() - ph->t0, ph->id);
		usleep(ph->time_to_sleep * 1000);
	}
}
