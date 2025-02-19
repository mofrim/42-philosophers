/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 12:35:39 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/07 16:26:10 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

/* printf */
# include <stdio.h>

/* guess what... */
# include <semaphore.h>

/* O_CREAT */
# include <fcntl.h>

/* for everything thread related */
# include <pthread.h>

/* for malloc */
# include <stdlib.h>

/* gettimeofday */
# include <sys/time.h>

/* usleep(), fork */
# include <unistd.h>

/* waitpid */
# include <sys/wait.h>

/* errno */
# include <errno.h>

/* memset */
# include <string.h>

/* SEM_MAX_VALUE */
# include <limits.h>

# define HELPTEXT "usage: ./philo num_of_philos ttd tte tts \
(numoftimes_to_eat)\n       - All values > 0!!!  >:(\n \
      - num_of_philos <= 100000\n"

/* i want a boolean, just for fun */
typedef int	t_bool;
# define FALSE 0
# define TRUE 1

typedef struct s_semas
{
	sem_t	*death;
	sem_t	*deathcheck;
	sem_t	*forks;
	sem_t	*fed;
	sem_t	*fedcheck;
	sem_t	*print;
	sem_t	*state;
}	t_semas;

/*
 * status:
 * 	- 0: normal / alive
 * 	- 1: fed
 * 	- 2: dead
 */
typedef struct s_philo
{
	long int	t0;
	long int	last_meal_start;
	int			philno;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			num_of_meals;
	int			max_meals;
	int			status;
	int			id;
	sem_t		*death;
	sem_t		*deathcheck;
	sem_t		*fed;
	sem_t		*fedcheck;
	sem_t		*forks;
	sem_t		*print;
	sem_t		*state;
	t_semas		*semas;
}	t_philo;

int			ft_atoi(char *s);
long int	gettime(void);
void		print_philo_data(t_philo p);
void		*philo(void	*phv);
t_bool		check_invalid_params(char **av, int ac);
t_philo		*make_proto_philo(int ac, char **av, t_semas *semas);
void		philo_dinner(t_philo *pp);
void		philo_eat(t_philo *pp);
t_bool		any_dead(t_philo *p);
void		init_semas(t_semas *semas, int philno);
t_bool		all_fed(t_philo *p);
int			print_logmsg(char *msg, t_philo *ph);
void		*death_thread(void *arg);
void		start_death_thread(pthread_t *kthread, t_philo *pp);
void		set_state_locked_int(int *statevar, int val, t_philo *ph);
void		set_state_locked_long(long *statevar, long val, t_philo *ph);

#endif
