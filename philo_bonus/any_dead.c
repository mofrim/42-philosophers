/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   any_dead.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 11:10:30 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/15 12:36:09 by fmaurer          ###   ########.fr       */
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
