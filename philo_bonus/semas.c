/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semas.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 15:44:00 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/07 15:30:10 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	init_semas(t_semas *semas, int philno)
{
	semas->forks = sem_open("/forks", O_CREAT, 0644, philno);
	semas->death = sem_open("/death", O_CREAT, 0644, SEM_VALUE_MAX - 1);
	semas->deathcheck = sem_open("/deathcheck", O_CREAT, 0644, 1);
	semas->state = sem_open("/state", O_CREAT, 0644, 1);
	semas->fed = sem_open("/fed", O_CREAT, 0644, SEM_VALUE_MAX - philno);
	semas->fedcheck = sem_open("/fedcheck", O_CREAT, 0644, 1);
	semas->print = sem_open("/print", O_CREAT, 0644, 1);
	if (semas->forks == SEM_FAILED || semas->death == SEM_FAILED || \
			semas->deathcheck == SEM_FAILED || semas->fed == SEM_FAILED || \
			semas->fedcheck == SEM_FAILED || semas->print == SEM_FAILED || \
			semas->state == SEM_FAILED)
	{
		printf("Opening sem failed, errno: %d\n", errno);
		exit(errno);
	}
}
