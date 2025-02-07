/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 20:42:36 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/07 12:20:40 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ph_eat(t_philo *ph, int id);
static void	ph_sleep(t_philo *ph, int id);
static void	ph_think(t_philo *ph, int id);

/** The main philosopher thread routine.
 *
 * The transfer of the id is a bit of a hack. But otherwise i would have gotten
 * into problems with my single philo struct approach. The only purpose of the
 * ph->init_lock mutex this is.
 */
void	*philo(void *phv)
{
	t_philo	*ph;
	int		id;

	ph = (t_philo *)phv;
	id = ph->id;
	pthread_mutex_unlock(ph->init_lock);
	while (1)
	{
		if (!ph->num_of_meals[id - 1] && (id % 2))
			usleep((ph->time_to_eat / 2) * 1000);
		if (ph->status[id - 1] != 0)
			return (NULL);
		ph_eat(ph, id);
		ph_sleep(ph, id);
		ph_think(ph, id);
	}
}

/* Waiting for the fork function. Try to get a mutex_lock on one fork. The
 * little bit smelly `first` flag is a side-effect of this abstraction.
 * Otherwise we will not be able to pick right/left fork using this function.
 */
static void	wait_for_fork(t_philo *ph, int *gotfork, int id, int first)
{
	if (first)
		pthread_mutex_lock(&ph->forks[id - 1]);
	else
		pthread_mutex_lock(&ph->forks[id % ph->philno]);
	(*gotfork)++;
	if (!any_dead(ph))
		print_logmsg(id, "has taken a fork", ph);
}

/**
 * The eating function of philo.
 *
 * This is the most complex part of a philos life.
 *
 * To avoid corrupting mutex state by mutliple mutex_unlocks, i intruduced the
 * 'gotfork' flag. This ensures even in case we enter this function in a already
 * dead state (which could happen if philo dies during initial sleep) that
 * mutex_unlock is not called on a not locked mutex. This could lead to errors
 * later especially mutex_destroy reports errno 16 (EBUSY) with mutexes like
 * this.
 */
void	ph_eat(t_philo *ph, int id)
{
	int		gotfork;

	gotfork = 0;
	if (!any_dead(ph))
		wait_for_fork(ph, &gotfork, id, 1);
	if (ph->philno != 1 && !any_dead(ph))
		wait_for_fork(ph, &gotfork, id, 0);
	if (ph->philno != 1 && !ph->status[id - 1] && !any_dead(ph) && gotfork == 2)
	{
		print_logmsg(id, "is eating", ph);
		ph->last_meal_start[id - 1] = gettime();
		usleep (ph->time_to_eat * 1000);
		ph->num_of_meals[id - 1]++;
		if (ph->max_meals && ph->num_of_meals[id - 1] == ph->max_meals)
			ph->status[id - 1] = 1;
	}
	if (ph->philno != 1 && gotfork)
		pthread_mutex_unlock(&ph->forks[id - 1]);
	if (gotfork == 2)
		pthread_mutex_unlock(&ph->forks[id % ph->philno]);
}

/* Sleeping function nothing interesting happening here. */
void	ph_sleep(t_philo *ph, int id)
{
	if (ph->philno != 1 && !ph->status[id - 1] && !any_dead(ph))
	{
		print_logmsg(id, "is sleeping", ph);
		usleep(ph->time_to_sleep * 1000);
	}
}

/* Thinking function. The thinking syncronization is not really needed for
 * manda, because mutexes_lock seems to almost certainly work with the FIFO
 * principle. For explanation see my PDF. */
void	ph_think(t_philo *ph, int id)
{
	if (ph->philno != 1 && !ph->status[id - 1] && !any_dead(ph))
	{
		print_logmsg(id, "is thinking", ph);
		if (ph->philno % 2 && (ph->time_to_sleep < 2 * ph->time_to_eat))
			usleep((ph->time_to_eat * 2 - ph->time_to_sleep) * 1000);
	}
}

// This is one of the warnings TSAN spit out before i implemented min/max
// ordering for the fork pickup on main-branch:

// WARNING: ThreadSanitizer: lock-order-inversion (potential deadlock) (pid=2822770)
//   Cycle in lock order graph: M0 (0x722800000028) => M1 (0x722800000050) => M2 (0x722800000078) => M3 (0x722800000000) => M0
//
//   Mutex M1 acquired here while holding mutex M0 in thread T2:
//     #0 pthread_mutex_lock ??:? (philo+0x75add)
//     #1 wait_for_fork /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/philo_routine.c:? (philo+0xfc241)
//     #2 ph_eat /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/philo_routine.c:81 (philo+0xfc241)
//     #3 philo /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/philo_routine.c:40 (philo+0xfc241)
//
//     Hint: use TSAN_OPTIONS=second_deadlock_stack=1 to get more informative warning message
//
//   Mutex M2 acquired here while holding mutex M1 in thread T3:
//     #0 pthread_mutex_lock ??:? (philo+0x75add)
//     #1 wait_for_fork /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/philo_routine.c:? (philo+0xfc241)
//     #2 ph_eat /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/philo_routine.c:81 (philo+0xfc241)
//     #3 philo /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/philo_routine.c:40 (philo+0xfc241)
//
//   Mutex M3 acquired here while holding mutex M2 in thread T4:
//     #0 pthread_mutex_lock ??:? (philo+0x75add)
//     #1 wait_for_fork /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/philo_routine.c:? (philo+0xfc241)
//     #2 ph_eat /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/philo_routine.c:81 (philo+0xfc241)
//     #3 philo /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/philo_routine.c:40 (philo+0xfc241)
//
//   Mutex M0 acquired here while holding mutex M3 in thread T1:
//     #0 pthread_mutex_lock ??:? (philo+0x75add)
//     #1 wait_for_fork /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/philo_routine.c:? (philo+0xfc241)
//     #2 ph_eat /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/philo_routine.c:81 (philo+0xfc241)
//     #3 philo /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/philo_routine.c:40 (philo+0xfc241)
//
//   Thread T2 (tid=2822774, running) created by main thread at:
//     #0 pthread_create ??:? (philo+0x757d2)
//     #1 init_philos /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/init_philo.c:52 (philo+0xfc9eb)
//     #2 main /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/philo.c:32 (philo+0xfaef8)
//
//   Thread T3 (tid=2822775, running) created by main thread at:
//     #0 pthread_create ??:? (philo+0x757d2)
//     #1 init_philos /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/init_philo.c:52 (philo+0xfc9eb)
//     #2 main /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/philo.c:32 (philo+0xfaef8)
//
//   Thread T4 (tid=2822776, running) created by main thread at:
//     #0 pthread_create ??:? (philo+0x757d2)
//     #1 init_philos /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/init_philo.c:52 (philo+0xfc9eb)
//     #2 main /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/philo.c:32 (philo+0xfaef8)
//
//   Thread T1 (tid=2822773, running) created by main thread at:
//     #0 pthread_create ??:? (philo+0x757d2)
//     #1 init_philos /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/init_philo.c:52 (philo+0xfc9eb)
//     #2 main /home/frido/c0de/42/theCore/10-philosophers/philoGH/philo/philo.c:32 (philo+0xfaef8)
//
// SUMMARY: ThreadSanitizer: lock-order-inversion (potential deadlock) ??:? in pthread_mutex_lock
