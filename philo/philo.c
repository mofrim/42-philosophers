/* ************************************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 11:53:00 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/24 19:08:07 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		any_dead(t_philo *ph);
void	cleanup(t_philo *ph, t_params *params);
int		all_are_fed(t_philo *ph);
int		watch_philos(t_philo *ph);

int	main(int ac, char **av)
{
	t_philo		*philos;
	t_params	*params;
	int			wellbeing;

	wellbeing = 0;
	if (5 <= ac && ac <= 6)
	{
		params = parse_params(ac, av);
		philos = init_philos(*params);
	}
	else
		return (printf(HELPTEXT), 1);
	if (!philos)
		return (printf("Init error!"), 22);
	wellbeing = watch_philos(philos);
	cleanup(philos, params);
	return (wellbeing);
}

int	watch_philos(t_philo *ph)
{
	while (1)
	{
		usleep(9000);
		if (all_are_fed(ph))
		{
			printf("All are fed after %d meals\n", ph->max_meals);
			return (0);
		}
		if (any_dead(ph))
		{
			printf("One philo dead. Ending simulation.\n");
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
		if (ph->status[i] != 1)
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

	philo_died = 0;
	time = gettime();
	i = -1;
	while (++i < ph->philno)
	{
		if (ph->status[i] == 2)
			return (1);
		if (time - ph->last_meal_start[i] > ph->time_to_die && \
				ph->status[i] == 0)
			philo_died = printf("%ld %d died\n", time - ph->t0, i + 1);
	}
	if (philo_died)
	{
		i = -1;
		while (++i < ph->philno)
			ph->status[i] = 2;
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
void	cleanup(t_philo *ph, t_params *params)
{
	int	i;

	i = -1;
	while (++i < ph->philno)
	{
		pthread_mutex_unlock(&(ph->forks)[i]);
		pthread_join(ph->phil_threads[i], NULL);
		pthread_mutex_destroy(&(ph->forks)[i]);
	}
	pthread_mutex_destroy(ph->init_lock);
	free(ph->num_of_meals);
	free(ph->last_meal_start);
	free(ph->status);
	free(ph->print_lock);
	free(ph->init_lock);
	free(ph->phil_threads);
	free(ph->forks);
	free(ph);
	free(params);
}
