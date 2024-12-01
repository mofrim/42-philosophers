/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 11:53:00 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/01 12:52:17 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	pthread_t	p1;
	pthread_t	p2[1000];
	int			num_of_philos;
	int			ttd;


	if (ac == 5)
	{


	}
	else
	{
		printf("usage: %s num_of_philos ttd tte tts (numoftimes_to_eat)\n", av[0]);
	}

	printf("sizeof(p1) = %lu\n", sizeof(p1));
	printf("sizeof(p1) = %lu\n", sizeof(p2));

	return (0);
}
