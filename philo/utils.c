/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:03:17 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/23 15:40:58 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* All-in-one atoi, isspace & isdigit. */
int	ft_atoi(char *s)
{
	int	num;
	int	sgn;

	num = 0;
	sgn = 1;
	while (*s == '\f' || *s == '\n' || *s == '\r' || *s == '\t'
		|| *s == ' ' || *s == '\v')
		s++;
	if (*s == '-')
		sgn = -1;
	if (*s == '-' || *s == '+')
		s++;
	while (48 <= *s && *s <= 57)
		num = num * 10 + *s++ - '0';
	return (num * sgn);
}

/* Get current time in milliseconds since epoch. */
long	gettime(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/* Check cmdline params. Currently philno <= 1000000. */
int	check_invalid_params(char **av, int ac)
{
	int	philno;

	philno = ft_atoi(av[1]);
	if (1 > philno || philno > 1000000)
		return (-1);
	if (ft_atoi(av[2]) <= 0)
		return (-1);
	if (ft_atoi(av[3]) <= 0)
		return (-1);
	if (ft_atoi(av[4]) <= 0)
		return (-1);
	if (ac == 6)
		if (ft_atoi(av[5]) <= 0)
			return (-1);
	return (0);
}

/* Parse the cmdline args into philo params struct for further processing. */
t_params	*parse_params(int ac, char **av)
{
	t_params	*params;

	if (check_invalid_params(av, ac) == -1)
		return (NULL);
	params = malloc(sizeof(t_params));
	if (params == NULL)
		return (NULL);
	params->philno = ft_atoi(av[1]);
	params->ttd = ft_atoi(av[2]);
	params->tte = ft_atoi(av[3]);
	params->tts = ft_atoi(av[4]);
	params->maxmeals = 0;
	if (ac == 6)
		params->maxmeals = ft_atoi(av[5]);
	return (params);
}

/**
 * The magic print_logmsg func.
 *
 * This was a bit enigmatic. There is a huge difference between calling the
 * `gettime` function before the mutex_lock or after it. With `gettime` before
 * the timestamps still got mixed up (but only by 1ms!)!
 * What was happening?
 * So when i finally implemented the mutexed printing i was really puzzled why
 * the mixed up timestamps still occured as if there was no mutexed printing (my
 * very first and long standing version). So, this is how i think this happened:
 * 1) Thread X finally locks the fork mutex and calls gettime.
 * 2) Meanwhile another thread Y locks another fork and calls gettime, but 1ms
 * later!
 * 3) Somehow the for thread Y the gettime finishes earlier or the scheduler
 * gives him priority for printing, voilà: mixed up timestamps.
 *
 * If we move the gettime into print_lock mutexed section in principle this
 * might be a bit problematic too, because now, it might happen that the thread
 * has to wait for the print lock so long that the actual timestamp is a couple
 * of ms off from the real moment of locking the fork mutex. But, i think this
 * is neglectable. In this case.
 */
int	print_logmsg(int id, char *msg, t_philo *ph)
{
	long	timestamp;

	pthread_mutex_lock(ph->print_lock);
	timestamp = gettime() - ph->t0;
	printf("%ld %d %s\n", timestamp, id, msg);
	pthread_mutex_unlock(ph->print_lock);
	return (1);
}
