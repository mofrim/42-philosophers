/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_clock.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 19:57:50 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/24 10:29:01 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*clock_routine(void *c)
{
	t_clock	*clk;
	
	clk = (t_clock *)c;
	while (!*clk->clock_stop)
	{
		usleep(1000);
		pthread_mutex_unlock(clk->clock_lock);
		pthread_mutex_lock(clk->clock_lock);
		*clk->the_clock = gettime() - clk->t0;
		pthread_mutex_unlock(clk->clock_lock);
	}
	return (NULL);
}

long	get_the_clock_time(t_clock *c)
{
	long	time;

	pthread_mutex_lock(c->clock_lock);
	time = *c->the_clock;
	pthread_mutex_unlock(c->clock_lock);
	return (time);
}

pthread_t	*run_the_clock_thread(t_clock *c)
{
	pthread_t	*clock_thread;
	
	clock_thread = malloc(sizeof(pthread_t));
	memset(clock_thread, 0, sizeof(pthread_t));
	pthread_create(clock_thread, 0, clock_routine, (void *)c);
	return (clock_thread);
}

int	init_clock(t_clock **clock)
{
	t_clock	*cl;

	*clock = malloc(sizeof(t_clock));
	cl = *clock;
	if (!cl)
		return (-1);
	cl->t0 = 0;
	cl->the_clock = malloc(sizeof(long));
	*cl->the_clock = 0;
	cl->clock_stop = malloc(sizeof(int));
	*cl->clock_stop = 0;
	cl->clock_lock = malloc(sizeof(pthread_mutex_t));
	memset(cl->clock_lock, 0, sizeof(pthread_mutex_t));
	if (pthread_mutex_init(cl->clock_lock, NULL) != 0)
		return (-1);
	return (0);
}
