/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_state_locked.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:44:58 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/07 15:46:55 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void set_state_locked_int(int *statevar, int val, t_philo *ph)
{
	sem_wait(ph->state);
	*statevar = val;
	sem_post(ph->state);
}

void set_state_locked_long(long *statevar, long val, t_philo *ph)
{
	sem_wait(ph->state);
	*statevar = val;
	sem_post(ph->state);
}
