/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:43:22 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/28 12:24:29 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	wait_for_fork(t_philo *pp, int *gotfork)
{
	sem_wait(pp->forks);
	(*gotfork)++;
	if (!any_dead(pp))
		print_logmsg("has taken a fork", pp);
}

static void	increment_fed_sema(t_philo pp)
{
	sem_wait(pp.fedcheck);
	sem_post(pp.fed);
	sem_post(pp.fedcheck);
}

void	philo_eat(t_philo *pp)
{
	int			gotfork;

	gotfork = 0;
	if (!any_dead(pp))
		wait_for_fork(pp, &gotfork);
	if (pp->philno != 1 && !any_dead(pp))
		wait_for_fork(pp, &gotfork);
	if (pp->philno != 1 && !any_dead(pp) && gotfork == 2)
	{
		print_logmsg("is eating", pp);
		pp->last_meal_start = gettime() - pp->t0;
		usleep (pp->time_to_eat * 1000);
		pp->num_of_meals++;
		if (pp->max_meals && pp->num_of_meals == pp->max_meals)
		{
			increment_fed_sema(*pp);
			pp->status = 1;
		}
	}
	if (pp->philno != 1 && gotfork)
		sem_post(pp->forks);
	if (gotfork == 2)
		sem_post(pp->forks);
}
