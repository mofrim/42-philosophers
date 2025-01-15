/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 12:35:39 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/08 23:31:45 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/* printf */
# include <stdio.h>

/* guess what... */
#include <semaphore.h>

/* O_CREAT */
#include <fcntl.h>

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

# include <limits.h>

/* i want a boolean, just for fun */
typedef int	bool;
# define false 0
# define true 1

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
}	t_philo;

int			ft_atoi(char *s);
long int	gettime(void);
void		print_philo_data(t_philo p);
void		*philo(void	*phv);
bool		check_invalid_params(char **av, int ac);
#endif
