/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 22:19:02 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/14 11:13:22 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Guess what? */
int	min(int a, int b)
{
	return ((a <= b) * a + (b < a) * b);
}

/* Huh!? */
int	max(int a, int b)
{
	return ((a >= b) * a + (b > a) * b);
}

/* Convenience function for state mutex handling. */
void	state_lock(t_philo *ph)
{
	pthread_mutex_lock(ph->state_lock);
}

/* Convenience function for state mutex handling. */
void	state_unlock(t_philo *ph)
{
	pthread_mutex_unlock(ph->state_lock);
}
