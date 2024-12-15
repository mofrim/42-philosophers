/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 11:53:00 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/15 20:47:49 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		any_dead(t_philo *ph);
void	cleanup(t_philo **ph);
int		all_are_dead_or_fed(t_philo *ph);

int	main(int ac, char **av)
{
	t_philo	*philos;

	if (5 <= ac && ac <= 6)
		philos = init_philos(av, ac);
	else
		return (22 * !nullmsg("usage: ./philo num_of_philos ttd tte tts" \
					" (numoftimes_to_eat)"));
	if (!philos)
		return (22 * !nullmsg("Init error!"));
	while (1)
	{
		usleep(9000);
		any_dead(philos);
		if (all_are_dead_or_fed(philos))
		{
			printf("All are dead or fed.\n");
			cleanup(&philos);
			return (1);
		}
	}
}

int	all_are_dead_or_fed(t_philo *ph)
{
	int	i;

	i = -1;
	while (++i < ph[0].num_of_philos)
		if (ph[i].status == 0)
			return (0);
	return (1);
}

int	any_dead(t_philo *ph)
{
	int			i;
	long int	time;
	int			philo_died;

	i = -1;
	time = gettime();
	philo_died = 0;
	while (++i < ph[0].num_of_philos)
	{
		if (time - ph[i].last_meal_start > ph[0].time_to_die && \
				ph[i].status == 0)
		{
			ph[i].status = 2;
			printf("%ld %d died\n", time - ph[i].t0, ph[i].id);
			philo_died = 1;
		}
	}
	if (philo_died)
		return (1);
	return (0);
}

// from the man ->
// 
// pthread_mutex_destroy: destroys a mutex object, freeing the resources it
// might hold.  The mutex must be unlocked on entrance. ** In the Linux‐ Threads
// implementation,  no  resources are associated with mutex objects, thus
// pthread_mutex_destroy actually does nothing except checking that the mutex is
// unlocked. **
//
// -> so, mutex_destroy does nothing execept checking if mutex is unlocked on
// linux!
void	cleanup(t_philo **ph)
{
	int	i;
	int	err;
	int	num_of_philos;

	i = 0;
	num_of_philos = ph[0]->num_of_philos;
	while (i < num_of_philos)
	{
		err = pthread_mutex_destroy(&ph[0]->forks[i]);
		if (err != 0)
		{
			printf("mutex destroy failed for philo %d\n", i + 1);
			printf("err = %d\n", err);
		}
		err = pthread_mutex_destroy(&ph[0]->forks[i]);
		pthread_join(ph[0]->phil_threads[i], NULL);
		i++;
	}
	free(ph[0]->phil_threads);
	free(ph[0]->forks);
	free(*ph);
}
