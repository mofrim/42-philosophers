/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 11:53:00 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/15 12:00:51 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	initial_unlink_semas(void);
static void	cleanup(sem_t *forks, sem_t *death, sem_t *deathcheck, t_philo *pp);

int	main(int ac, char **av)
{
	sem_t	*forks;
	sem_t	*death;
	sem_t	*deathcheck;
	int		philno;
	t_philo	*proto_philo;

	initial_unlink_semas();
	if (!((5 <= ac && ac <= 6) && !check_invalid_params(av, ac)))
		return (printf("usage: ./philo num_of_philos ttd tte tts" \
					" (numoftimes_to_eat)"), 22);
	philno = ft_atoi(av[1]);
	forks = sem_open("/forks", O_CREAT, 0644, philno);
	death = sem_open("/death", O_CREAT, 0644, SEM_VALUE_MAX - 1);
	deathcheck = sem_open("/deathcheck", O_CREAT, 0644, 1);
	if (forks == SEM_FAILED || death == SEM_FAILED || deathcheck == SEM_FAILED)
	{
		printf("Opening sem failed, errno: %d\n", errno);
		exit(errno);
	}
	proto_philo = parse_philo(ac, av, death, deathcheck);
	summon_philos(philno, proto_philo, forks);
	while (waitpid(-1, NULL, 0) != -1)
		;
	cleanup(forks, death, deathcheck, proto_philo);
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
 */
static void	initial_unlink_semas(void)
{
	sem_unlink("/forks");
	sem_unlink("/death");
	sem_unlink("/deathcheck");
}

/* Well, cleanup. */
static void	cleanup(sem_t *forks, sem_t *death, sem_t *deathcheck, t_philo *pp)
{
	sem_close(forks);
	sem_close(death);
	sem_close(deathcheck);
	sem_unlink("/forks");
	sem_unlink("/death");
	sem_unlink("/deathcheck");
	free(pp);
}
