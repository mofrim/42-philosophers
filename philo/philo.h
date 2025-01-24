/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 12:35:39 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/24 10:07:35 by fmaurer          ###   ########.fr       */
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

typedef struct s_clock
{
	long			t0;
	long			*the_clock;
	int 			*clock_stop;
	pthread_mutex_t *clock_lock;
}	t_clock;

typedef struct s_philo
{
	long int		t0;
	long int		last_meal_start;
	int				philno;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_of_meals;
	int				max_meals;
	int				status;
	int				id;
	pthread_t		*phil_threads;
	pthread_mutex_t	*forks;
	t_clock			*clock;
	pthread_t		*clock_thread;
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

void		*clock_routine(void *c);
long		get_the_clock_time(t_clock *c);
pthread_t	*run_the_clock_thread(t_clock *c);
int			init_clock(t_clock **clock);
#endif
