/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 11:53:00 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/24 12:07:57 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		any_dead(t_philo *ph);
void	cleanup(t_philo **ph);
int		all_are_fed(t_philo *ph);

int	main(int ac, char **av)
{
	t_philo		*philos;
	t_params	*params;

	if (5 <= ac && ac <= 6)
	{
		params = parse_params(ac, av);
		philos = init_philos(*params);
	}
	else
		return (printf(HELPTEXT), 1);
	if (!philos)
		return (printf("Init error!"), 22);
	while (1)
	{
		usleep(9000);
		if (any_dead(philos) || all_are_fed(philos))
		{
			if (all_are_fed(philos))
				printf("All are fed after %d meals\n", philos->max_meals);
			else
				printf("One philo dead. Ending simulation.\n");
			cleanup(&philos);
			return (1);
		}
	}
}

/**
 * Check if all philo are fed, meaning, have reached max_meals number of meals.
 * Return 1 if so, else 0.
 */
int	all_are_fed(t_philo *ph)
{
	int	i;

	i = -1;
	while (++i < ph[0].philno)
		if (ph[i].status != 1)
			return (0);
	return (1);
}

/**
 * Check if any philo did not eat ttd since last_meal_start. If one of them
 * dies, set all philos stati to dead, hence making them cleanly finish their
 * threads. 
 */
int	any_dead(t_philo *ph)
{
	int			i;
	long int	time;
	int			philo_died;

	i = -1;
	time = gettime();
	philo_died = 0;
	while (++i < ph->philno)
		if (time - ph->last_meal_start > ph[0].time_to_die && \
				ph->status == 0)
			philo_died = printf("%ld %d died\n", time - ph->t0, *ph->id);
	if (philo_died)
	{
		i = -1;
		while (++i < ph[0].philno)
			ph->status = 2;
		return (1);
	}
	return (0);
}

/**
 * Cleanup all data structures.
 *
 * From the man ->
 * 
 * pthread_mutex_destroy: destroys a mutex object, freeing the resources it
 * might hold.  The mutex must be unlocked on entrance. ** In the Linux‐ Threads
 * implementation,  no  resources are associated with mutex objects, thus
 * pthread_mutex_destroy actually does nothing except checking that the mutex is
 * unlocked. **
 */
void	cleanup(t_philo **ph)
{
	int	i;
	int	num_of_philos;

	num_of_philos = (*ph)->philno;
	i = -1;
	while (++i < num_of_philos)
	{
		pthread_mutex_unlock(&((*ph)->forks)[i]);
		pthread_join((*ph)->phil_threads[i], NULL);
		pthread_mutex_destroy(&((*ph)->forks)[i]);
	}
	i = -1;
	free((*ph)->phil_threads);
	free((*ph)->forks);
	free(*ph);
}
