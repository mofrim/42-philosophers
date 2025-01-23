/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 12:35:39 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/23 14:25:25 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/* printf */
# include <stdio.h>

/* for everything thread related */
# include <pthread.h>

/* for malloc */
# include <stdlib.h>

/* gettimeofday */
# include <sys/time.h>

/* usleep(),  */
# include <unistd.h>

/* memset */
# include <string.h>

# define HELPTEXT "usage: ./philo num_of_philos ttd tte tts \
	(numoftimes_to_eat)\nAll values > 0!!!  >:("

typedef struct s_philo
{
	long int		t0;
	long int		last_meal_start;
	int				num_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_of_meals;
	int				max_meals;
	int				status;
	int				id;
	pthread_t		*phil_threads;
	pthread_mutex_t	*forks;
}	t_philo;

int			ft_atoi(char *s);
long int	gettime(void);
void		print_philo_data(t_philo p);
void		*philo(void	*phv);
t_philo		*init_philos(char **av, int ac, int philno);
void		*nullmsg(char *msg);
#endif
