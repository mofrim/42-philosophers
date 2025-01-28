/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 11:53:00 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/28 14:14:51 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* Well... this is weird. When printing straigt to a terminal there was no need
 * for semaphored printng function `print_logmsg`. But when redirecting output
 * to a file timestamps get completely messed up due to the subprocesses and
 * printf's full-buffered mode in this case. This still happens with
 * `print_logmsg` being used now but can be worked around by using one of the
 * following commands when calling `philo_bonus`:
 * 
 * - `script -q c "./philo_bonus 400 500 100 100 10` log.log
 * - `stdbuf -oL ./philo_bonus 400 500 100 100 10 > log.log`
 *
 * These command force line-buffered output again.
 */

static void	initial_unlink_semas(void);
static void	cleanup(t_semas *semas, t_philo *pp);
void		main_death_watchdog(int *cpids, t_philo *ph);

int	main(int ac, char **av)
{
	t_semas	*semas;
	t_philo	*proto_philo;

	initial_unlink_semas();
	if (!((5 <= ac && ac <= 6) && !check_invalid_params(av, ac)))
		return (printf("usage: ./philo num_of_philos ttd tte tts" \
					" (numoftimes_to_eat)"), 22);
	semas = malloc(sizeof(t_semas));
	init_semas(semas, ft_atoi(av[1]));
	proto_philo = make_proto_philo(ac, av, semas);
	philo_dinner(proto_philo);
	while (waitpid(-1, NULL, 0) != -1)
		;
	if (all_fed(proto_philo))
		printf("All are fed!\n");
	else
		printf("A philo died!\n");
	cleanup(semas, proto_philo);
	return (0);
}

/**
 * Unlink all semas we are about to use.
 *
 * So... due to the fact that we are not allowed to use signal handlers, if we
 * terminate our program by hitting Ctrl-C or anything there is no cleanup done
 * in this case. Meaning: the semaphores will not get unlinked which will block
 * the execution of the program the next time it is run. In order to prevent
 * this just bluntly unlink them before we do anything.
 * BTW: the semas a *real* objects in the filesystem and can be found ander
 * `/dev/shm/sem.SEMNAME` ;)
 */
static void	initial_unlink_semas(void)
{
	sem_unlink("/forks");
	sem_unlink("/death");
	sem_unlink("/deathcheck");
	sem_unlink("/fed");
	sem_unlink("/fedcheck");
	sem_unlink("/print");
}

/* Well, cleanup. */
static void	cleanup(t_semas *semas, t_philo *pp)
{
	sem_close(semas->forks);
	sem_close(semas->death);
	sem_close(semas->deathcheck);
	sem_close(semas->fed);
	sem_close(semas->fedcheck);
	sem_close(semas->print);
	sem_unlink("/forks");
	sem_unlink("/death");
	sem_unlink("/deathcheck");
	sem_unlink("/fed");
	sem_unlink("/fedcheck");
	sem_unlink("/print");
	free(semas);
	free(pp);
}
