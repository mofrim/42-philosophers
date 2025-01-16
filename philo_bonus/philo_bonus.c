/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 11:53:00 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/16 12:07:16 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	initial_unlink_semas(void);
static void	cleanup(t_semas *semas, t_philo *pp);
void		main_death_watchdog(int *cpids, t_philo *ph);

int	main(int ac, char **av)
{
	t_semas	*semas;
	t_philo	*proto_philo;

	semas = malloc(sizeof(t_semas));
	initial_unlink_semas();
	if (!((5 <= ac && ac <= 6) && !check_invalid_params(av, ac)))
		return (printf("usage: ./philo num_of_philos ttd tte tts" \
					" (numoftimes_to_eat)"), 22);
	init_semas(semas, ft_atoi(av[1]));
	proto_philo = make_proto_philo(ac, av, semas);
	summon_philos(proto_philo);
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
 */
static void	initial_unlink_semas(void)
{
	sem_unlink("/forks");
	sem_unlink("/death");
	sem_unlink("/deathcheck");
	sem_unlink("/fed");
	sem_unlink("/fedcheck");
}

/* Well, cleanup. */
static void	cleanup(t_semas *semas, t_philo *pp)
{
	sem_close(semas->forks);
	sem_close(semas->death);
	sem_close(semas->deathcheck);
	sem_close(semas->fed);
	sem_close(semas->fedcheck);
	sem_unlink("/forks");
	sem_unlink("/death");
	sem_unlink("/deathcheck");
	sem_unlink("/fed");
	sem_unlink("/fedcheck");
	free(semas);
	free(pp);
}
