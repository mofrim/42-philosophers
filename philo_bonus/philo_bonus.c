/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 11:53:00 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/08 14:09:54 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_philo	*parse_philo(int ac, char **av, int philno);
void	summon_philos(int philno, sem_t *forks, t_philo *pp);

int	main(int ac, char **av)
{
	sem_t	*forks;
	int		philno;
	t_philo	*proto_philo;

	if ((5 <= ac && ac <= 6) && !check_invalid_params(av, ac))
		printf("go philo go!\n");
	else
		return (printf("usage: ./philo num_of_philos ttd tte tts" \
					" (numoftimes_to_eat)"), 22);
	philno = ft_atoi(av[1]);
	forks = sem_open("/forks", O_CREAT, 0644, philno);
	proto_philo = parse_philo(ac, av, philno);
	summon_philos(philno, forks, proto_philo);
	sem_close(forks);
	sem_unlink("/forks");

}

void	summon_philos(int philno, sem_t *forks, t_philo *pp)
{
	int	i;
	int	ppid;
	long int	time0;

	i = -1;

	time0 = gettime();
	while (++i < philno)
	{
		pp->id = i + 1;
		pp->t0 = time0;
		ppid = fork();
		if (ppid < 0)
			return ;
		if (ppid == 0)
		{
			if (pp->id % 2)
				usleep((pp->time_to_eat / 2) * 1000);
			printf("philo no.%d, time0: %ld\n", pp->id, pp->t0);
			exit(0);
		}
	}
}



t_philo	*parse_philo(int ac, char **av, int philno)
{
	t_philo *ph;
	int		i;
	long int	time0;

	ph = malloc(sizeof(t_philo));
	i = -1;
	time0 = gettime();
	while (++i < philno)
	{
		ph->id = i + 1;
		ph->t0 = time0;
		ph->philno = philno;
		ph->last_meal_start = time0;
		ph->num_of_philos = ft_atoi(av[1]);
		ph->time_to_die = ft_atoi(av[2]);
		ph->time_to_eat = ft_atoi(av[3]);
		ph->time_to_sleep = ft_atoi(av[4]);
		ph->num_of_meals = 0;
		ph->status = 0;
		if (ac == 6)
			ph->max_meals = ft_atoi(av[5]);
		else
			ph->max_meals = 0;
	}
	return (ph);
}
