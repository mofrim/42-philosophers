/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 11:53:00 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/08 17:47:44 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_philo	*parse_philo(int ac, char **av, int philno);
void	summon_philos(int philno, sem_t *forks, t_philo *pp);
void	*killer_thread(void *arg);

int	main(int ac, char **av)
{
	sem_t	*forks;
	int		philno;
	t_philo	*proto_philo;
	pid_t	dummy;

	// if philo_bonus from before ended with a crash or signal.
	sem_unlink("/forks");

	if ((5 <= ac && ac <= 6) && !check_invalid_params(av, ac))
		printf("go philo go!\n");
	else
		return (printf("usage: ./philo num_of_philos ttd tte tts" \
					" (numoftimes_to_eat)"), 22);
	philno = ft_atoi(av[1]);
	forks = sem_open("/forks", O_CREAT, 0644, philno);
	if (forks == SEM_FAILED)
	{
		printf("opening sem failed, errno: %d\n", errno);
		exit(errno);
	}
	proto_philo = parse_philo(ac, av, philno);
	dummy = fork();
	if (dummy < 0)
	{
		printf("fork failed!\n");
		exit(errno);
	}
	if (dummy == 0)
	{
		summon_philos(philno, forks, proto_philo);
		if(waitpid(-1, NULL, 0) != -1)
			printf("a philo has died!\n");
		// while ((waitpid(-1, NULL, 0) != -1) && errno != ECHILD)
		// 	;
		exit(0);
	}
	else
	{
		if(waitpid(-1, NULL, 0) != -1)
			printf("a philo has died!\n");
		kill(-dummy, SIGINT);
	}
	sem_close(forks);
	sem_unlink("/forks");
	return (0);
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
			pthread_t	killerthread;
			int			gotfork = 0;
			int long	meal_start;

			if (pthread_create(&killerthread, NULL, killer_thread, (void *)pp) != 0)
			{
				printf("Thread creation failed");
				exit(1);
			}
			pthread_detach(killerthread);

			if (pp->id % 2)
				usleep((pp->time_to_eat / 2) * 1000);
			// printf("philo no.%d, time0: %ld\n", pp->id, pp->t0);

			while (1)
			{
				//eat
				gotfork = 0;
				if (!pp->status)
				{
					gotfork = 1;
					sem_wait(forks);
					printf("%ld %d has taken a fork\n", gettime() - pp->t0, pp->id);
				}
				if (!pp->status)
				{
					gotfork++;
					sem_wait(forks);
					printf("%ld %d has taken a fork\n", gettime() - pp->t0, pp->id);
				}
				if (!pp->status && gotfork == 2)
				{
					meal_start = gettime() - pp->t0;
					printf("%ld %d is eating\n", meal_start, pp->id);
					pp->last_meal_start = meal_start;
					pp->num_of_meals++;
					usleep (pp->time_to_eat * 1000);
				}
				if (gotfork)
					sem_post(forks);
				if (gotfork == 2)
					sem_post(forks);

				// sleep
				if (!pp->status)
				{
					printf("%ld %d is sleeping\n", gettime() - pp->t0, pp->id);
					usleep (pp->time_to_sleep * 1000);
				}
				if (!pp->status)
					printf("%ld %d is thinking\n", gettime() - pp->t0, pp->id);

				if (pp->status)
				{
					printf("philo %d calling exit\n", pp->id);
					exit(0);
				}
			}
		}
	}
}

void	*killer_thread(void *arg)
{
	t_philo		*ph;
	long int	time;

	ph = (t_philo *)arg;
	while (1)
	{
		usleep(9000);
		time = gettime() - ph->t0;
		if (time - ph->last_meal_start >= ph->time_to_die)
		{
			printf("%ld %d died\n", time, ph->id);
			ph->status = 2;
		}
		if (ph->status)
			return (NULL);
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
