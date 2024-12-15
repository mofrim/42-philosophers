/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 12:35:39 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/15 11:51:08 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/* Printf */
# include <stdio.h>

/* For everything thread related */
# include <pthread.h>

/* For malloc */
# include <stdlib.h>

/* gettimeofday */
# include <sys/time.h>

/* usleep(),  */
# include <unistd.h>

/* memset */
# include <string.h>

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
t_philo		*init_philos(char **av, int argnum);
void		*nullmsg(char *msg);
#endif
