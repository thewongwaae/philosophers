/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwong <hwong@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 00:41:57 by hwong             #+#    #+#             */
/*   Updated: 2023/01/21 00:41:57 by hwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(int ms)
{
	long int	time;

	time = timestamp();
	while (timestamp() - time < ms)
		usleep(ms / 10);
}

int	is_dead(t_philo *philo, int nb)
{
	pthread_mutex_lock(&philo->info->dead);
	if (nb)
		philo->info->stop = 1;
	if (philo->info->stop)
	{
		pthread_mutex_unlock(&philo->info->dead);
		return (1);
	}
	pthread_mutex_unlock(&philo->info->dead);
	return (0);
}

void	output_philo(t_philo *philo, char *str, char *colour)
{
	long int	time;

	pthread_mutex_lock(&(philo->info->print));
	time = timestamp() - philo->info->start_time;
	if (!philo->info->stop && time >= 0 \
			&& time <= INT_MAX && !is_dead(philo, 0))
	{
		printf("%s", colour);
		printf("%lld\t%d", timestamp() - philo->info->start_time, philo->id);
		printf("%s", str);
		printf(RESET);
	}
	pthread_mutex_unlock(&(philo->info->print));
}

void	freeall(t_data *info)
{
	int	i;

	i = -1;
	while (++i < info->philo_count)
	{
		pthread_mutex_destroy(&info->philo[i].fork_l);
		pthread_mutex_destroy(info->philo[i].fork_r);
	}
	free(info->philo);
	pthread_mutex_destroy(&info->print);
	pthread_mutex_destroy(&info->m_stop);
	pthread_mutex_destroy(&info->m_eat);
	pthread_mutex_destroy(&info->dead);
}
