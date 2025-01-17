/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   any_dead_all_fed.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 11:10:30 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/17 22:45:48 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * Check if any philo has died.
 *
 * This is done by trying to increment the death sem. To avoid any false
 * overincrementation of the death sem only one process at a time is allowed to
 * check the death sem. If incrementation fails this means death sem was already
 * at SEM_VALUE_MAX, meaning a process did increment it without decrementing
 * afterwards, meaning called sem_post without calling sem_wait afterwards,
 * meaning: is dead.
 */
t_bool	any_dead(t_philo *p)
{
	int	is_one_dead;

	sem_wait(p->deathcheck);
	is_one_dead = sem_post(p->death);
	if (is_one_dead == -1)
	{
		sem_post(p->deathcheck);
		return (TRUE);
	}
	sem_wait(p->death);
	sem_post(p->deathcheck);
	return (FALSE);
}

/**
 * Check if all philos are fed.
 *
 * Same logic like any_dead but subtely different... the fed semaphore is
 * initialized to SEM_VALUE_MAX - philno. Every time a philo is fed up, it calls
 * sem_post on this sema. So if all are finally fed, any further sem_post to
 * this sema will return -1, which would be an indicator for fedness.
 *
 * Kept this of historic reason. In the current implementation this is not
 * necessary anymore.
 */
t_bool	all_fed(t_philo *p)
{
	int	fed_up;

	sem_wait(p->fedcheck);
	fed_up = sem_post(p->fed);
	if (fed_up == -1)
	{
		sem_post(p->fedcheck);
		return (TRUE);
	}
	sem_wait(p->fed);
	sem_post(p->fedcheck);
	return (FALSE);
}
