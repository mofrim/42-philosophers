/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 12:35:39 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/07 01:18:39 by fmaurer          ###   ########.fr       */
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
	void			*args;
	pthread_t		*phil_threads;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*state_lock;
}	t_philo;

typedef struct s_params
{
	long	ttd;
	long	tte;
	long	tts;
	int		maxmeals;
	int		philno;
}	t_params;

typedef struct s_phthread_arg
{
	int		id;
	t_philo	*ph;
}	t_phthread_arg;

int			ft_atoi(char *s);
long int	gettime(void);
void		print_philo_data(t_philo p);
void		*philo(void	*phv);
t_philo		*init_philos(t_params params);
void		*nullmsg(char *msg);
t_params	*parse_params(int ac, char **av);
int			any_dead(t_philo *ph);
int			print_logmsg(int id, char *msg, t_philo *ph);
int			min(int a, int b);
int			max(int a, int b);
void		state_lock(t_philo *ph);
void		state_unlock(t_philo *ph);
void		set_state_locked_int(int *statevar, int val, t_philo *ph);
void		set_state_locked_long(long *statevar, long val, t_philo *ph);
#endif
