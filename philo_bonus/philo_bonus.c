/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 11:53:00 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/08 23:36:04 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_philo	*parse_philo(int ac, char **av, sem_t *death, sem_t *deathcheck);
void	summon_philos(int philno, t_philo *pp, sem_t *forks);
bool	any_dead(t_philo *p);
void	*killer_thread(void *arg);

int	main(int ac, char **av)
{
	sem_t	*forks;
	sem_t	*death;
	sem_t	*deathcheck;
	int		philno;
	t_philo	*proto_philo;

	sem_unlink("/forks");
	sem_unlink("/death");
	sem_unlink("/deathcheck");
	if (!((5 <= ac && ac <= 6) && !check_invalid_params(av, ac)))
		return (printf("usage: ./philo num_of_philos ttd tte tts" \
					" (numoftimes_to_eat)"), 22);
	philno = ft_atoi(av[1]);
	forks = sem_open("/forks", O_CREAT, 0644, philno);
	death = sem_open("/death", O_CREAT, 0644, SEM_VALUE_MAX - 1);
	deathcheck = sem_open("/deathcheck", O_CREAT, 0644, 1);
	if (forks == SEM_FAILED || death == SEM_FAILED || deathcheck == SEM_FAILED)
	{
		printf("opening sem failed, errno: %d\n", errno);
		exit(errno);
	}
	proto_philo = parse_philo(ac, av, death, deathcheck);
	summon_philos(philno, proto_philo, forks);
	while (waitpid(-1, NULL, 0) != -1)
		;
	sem_close(forks);
	sem_close(death);
	sem_close(deathcheck);
	sem_unlink("/forks");
	sem_unlink("/death");
	sem_unlink("/deathcheck");
	free(proto_philo);
	return (0);
}

void	summon_philos(int philno, t_philo *pp, sem_t *forks)
{
	int			i;
	int			ppid;
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
			// wow! with the tinking trick you dont even need initial sleep!!!
			// TODO: understand that!
			//
			// if ((philno % 2) && pp->id % 2)
			// 	usleep((pp->time_to_eat / 2) * 1000);
			// if (!(philno % 2) && !(pp->id % 2))
			// 	usleep((pp->time_to_eat / 2) * 1000);

			while (1)
			{
				//eat
				gotfork = 0;
				if (!any_dead(pp))
				{
					sem_wait(forks);
					gotfork++;
					if (!pp->status)
						printf("%ld %d has taken a fork\n", gettime() - pp->t0, pp->id);
				}
				if (!any_dead(pp))
				{
					sem_wait(forks);
					gotfork++;
					if (!pp->status)
						printf("%ld %d has taken a fork\n", gettime() - pp->t0, pp->id);
				}
				if (!any_dead(pp) && gotfork == 2)
				{
					meal_start = gettime() - pp->t0;
					pp->last_meal_start = meal_start;
					printf("%ld %d is eating\n", meal_start, pp->id);
					usleep (pp->time_to_eat * 1000);
					pp->num_of_meals++;
					if (pp->max_meals && pp->num_of_meals == pp->max_meals)
						pp->status = 1;
				}
				if (gotfork)
					sem_post(forks);
				if (gotfork == 2)
					sem_post(forks);

				// sleep
				if (!any_dead(pp) && !pp->status)
				{
					printf("%ld %d is sleeping\n", gettime() - pp->t0, pp->id);
					usleep (pp->time_to_sleep * 1000);
				}
				// thinking... more important than i thought 8>
				if (!any_dead(pp) && !pp->status)
				{
					printf("%ld %d is thinking\n", gettime() - pp->t0, pp->id);
					if (pp->philno % 2)
						usleep((pp->time_to_eat * 2 - pp->time_to_sleep) * 1000);
				}

				if (any_dead(pp) || pp->status)
				{
					pthread_join(killerthread, NULL);
					sem_close(forks);
					sem_close(pp->death);
					sem_close(pp->deathcheck);
					free(pp);
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
			sem_post(ph->death);
			return (NULL);
		}
		if (any_dead(ph) || ph->status)
			return (NULL);
	}
}

t_philo	*parse_philo(int ac, char **av, sem_t *death, sem_t *deathcheck)
{
	t_philo *ph;
	int		i;
	long int	time0;
	int	philno;

	philno = ft_atoi(av[1]);
	ph = malloc(sizeof(t_philo));
	i = -1;
	time0 = gettime();
	while (++i < philno)
	{
		ph->id = i + 1;
		ph->t0 = time0;
		ph->death = death;
		ph->deathcheck = deathcheck;
		ph->philno = philno;
		ph->last_meal_start = time0;
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

bool	any_dead(t_philo *p)
{
	int	is_one_dead;

	sem_wait(p->deathcheck);
	is_one_dead = sem_post(p->death);
	if (is_one_dead == -1)
	{
		sem_post(p->deathcheck);
		return (true);
	}
	sem_wait(p->death);
	sem_post(p->deathcheck);
	return (false);
}
