/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: m4rrs <m4rrs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 14:53:25 by nnorazma          #+#    #+#             */
/*   Updated: 2022/12/26 17:24:36 by m4rrs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*check_death(void *phi)
{
	t_philo	*philo;

	philo = (t_philo *)phi;
	ft_usleep(philo->info->death + 1);
	pthread_mutex_lock(&philo->info->m_eat);
	pthread_mutex_lock(&philo->info->m_stop);
	if (!is_dead(philo, 0) && timestamp() - \
			philo->last_eat >= (long)(philo->info->death))
	{
		pthread_mutex_unlock(&philo->info->m_eat);
		pthread_mutex_unlock(&philo->info->m_stop);
		print(philo, " died\n", RED);
		is_dead(philo, 1);
	}
	pthread_mutex_unlock(&philo->info->m_eat);
	pthread_mutex_unlock(&philo->info->m_stop);
	return (NULL);
}

void	take_fork(t_philo *philo)
{
	pthread_mutex_lock(&(philo->fork_l));
	print(philo, " has taken a fork\n", GREEN);
	if (philo->info->n_philo == 1)
	{
		ft_usleep(philo->info->death * 2);
		return ;
	}
	pthread_mutex_lock((philo->fork_r));
	print(philo, " has taken a fork\n", GREEN);
}

void	philo_eat(t_philo *philo)
{
	print(philo, " is eating\n", CYAN);
	pthread_mutex_lock(&(philo->info->m_eat));
	philo->last_eat = timestamp();
	philo->m_count++;
	pthread_mutex_unlock(&(philo->info->m_eat));
	ft_usleep(philo->info->eat);
	pthread_mutex_unlock((philo->fork_r));
	pthread_mutex_unlock(&(philo->fork_l));
	print(philo, " is sleeping\n", PURPLE);
	ft_usleep(philo->info->sleep);
	print(philo, " is thinking\n", YELLOW);
}

void	*philo_life(void *phi)
{
	t_philo		*philo;
	pthread_t	t;

	philo = (t_philo *)phi;
	if (philo->id % 2 == 0)
		ft_usleep(philo->info->eat / 10);
	while (!is_dead(philo, 0))
	{
		pthread_create(&t, NULL, check_death, phi);
		take_fork(philo);
		philo_eat(philo);
		pthread_detach(t);
		if (philo->m_count == philo->info->n_eat)
		{
			pthread_mutex_lock(&philo->info->m_stop);
			if (++philo->info->philo_eat == philo->info->n_philo)
			{
				pthread_mutex_unlock(&philo->info->m_stop);
				is_dead(philo, 2);
			}
			pthread_mutex_unlock(&philo->info->m_stop);
			return (NULL);
		}
	}
	return (NULL);
}
