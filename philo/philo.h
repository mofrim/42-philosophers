/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 12:35:39 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/24 13:27:20 by fmaurer          ###   ########.fr       */
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
	long			t0;
	long			*last_meal_start;
	int				philno;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				*num_of_meals;
	int				max_meals;
	int				*status;
	int				id;
	pthread_t		*phil_threads;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*init_lock;
}	t_philo;

typedef struct s_params
{
	long	ttd;
	long	tte;
	long	tts;
	int		maxmeals;
	int		philno;
}	t_params;

int			ft_atoi(char *s);
long int	gettime(void);
void		print_philo_data(t_philo p);
void		*philo(void	*phv);
t_philo		*init_philos(t_params params);
void		*nullmsg(char *msg);
t_params	*parse_params(int ac, char **av);
int			any_dead(t_philo *ph);
#endif
