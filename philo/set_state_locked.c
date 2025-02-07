/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_state_locked.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 00:18:48 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/07 00:20:58 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Convenience functions for setting values while locking the state_lock mutex.
 */

void	set_state_locked_int(int *statevar, int val, t_philo *ph)
{
	state_lock(ph);
	*statevar = val;
	state_unlock(ph);
}

void	set_state_locked_long(long *statevar, long val, t_philo *ph)
{
	state_lock(ph);
	*statevar = val;
	state_unlock(ph);
}
