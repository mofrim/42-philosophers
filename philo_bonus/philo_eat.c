/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:43:22 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/15 10:53:38 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	wait_for_fork(t_philo *pp, sem_t *forks, int *gotfork)
{
	sem_wait(forks);
	(*gotfork)++;
	if (!pp->status)
		printf("%ld %d has taken a fork\n", gettime() - pp->t0, pp->id);
}

void	eat(t_philo *pp, sem_t *forks)
{
	int			gotfork;
	long int	meal_start;

	gotfork = 0;
	if (!any_dead(pp))
		wait_for_fork(pp, forks, &gotfork);
	if (!any_dead(pp))
		wait_for_fork(pp, forks, &gotfork);
	if (!any_dead(pp) && gotfork == 2)
	{
		meal_start = gettime() - pp->t0;
		pp->last_meal_start = meal_start;
		printf("%ld %d is eating\n", meal_start, pp->id);
		usleep (pp->time_to_eat * 1000);
		pp->num_of_meals++;
		if (pp->max_meals && pp->num_of_meals == pp->max_meals)
			pp->status = 1;
	}
	if (gotfork)
		sem_post(forks);
	if (gotfork == 2)
		sem_post(forks);
}
