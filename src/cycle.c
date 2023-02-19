/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwong <hwong@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 00:41:42 by hwong             #+#    #+#             */
/*   Updated: 2023/02/19 16:05:19 by hwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
*	Check if a philosopher has exceeded the death timer
*	if yes, set philo to dead
*/
void	*check_death(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	ft_usleep(philo->info->death + 1);
	pthread_mutex_lock(&philo->info->m_eat);
	pthread_mutex_lock(&philo->info->m_stop);
	if (!is_dead(philo, 0) && timestamp() - \
			philo->last_meal >= (long)(philo->info->death))
	{
		pthread_mutex_unlock(&philo->info->m_eat);
		pthread_mutex_unlock(&philo->info->m_stop);
		output_philo(philo, " died\n", RED);
		is_dead(philo, 1);
	}
	pthread_mutex_unlock(&philo->info->m_eat);
	pthread_mutex_unlock(&philo->info->m_stop);
	return (NULL);
}

/*
*	Lock left and right fork
*/
void	take_fork(t_philo *philo)
{
	pthread_mutex_lock(&(philo->fork_l));
	output_philo(philo, " has taken a fork\n", GREEN);
	if (philo->info->philo_count == 1)
	{
		ft_usleep(philo->info->death * 2);
		return ;
	}
	pthread_mutex_lock(philo->fork_r);
	output_philo(philo, " has taken a fork\n", GREEN);
}

/*
*	Lock eat mutex
*	Update last meal time
*	Increment meal count
*	usleep for meal time then release mutexes for both fork
*/
void	philo_eat(t_philo *philo)
{
	output_philo(philo, " is eating\n", CYAN);
	pthread_mutex_lock(&(philo->info->m_eat));
	philo->last_meal = timestamp();
	philo->meal_count++;
	pthread_mutex_unlock(&(philo->info->m_eat));
	ft_usleep(philo->info->eat);
	pthread_mutex_unlock((philo->fork_r));
	pthread_mutex_unlock(&(philo->fork_l));
	output_philo(philo, " is sleeping\n", PURPLE);
	ft_usleep(philo->info->sleep);
	output_philo(philo, " is thinking\n", YELLOW);
}

/*
*	The main logic loop for a philosopher life cycle
*	Wait for a short amount of time to prevent deadlock
*	- deadlock -> processes trying to use same resource (hang or crash)
*	Create thread to check for death and join to main thread
*	Locks forks, eats, unlocks forks
*	If a philo has reached required meal count, check other philo
*/
void	*life(void *p)
{
	t_philo		*philo;
	pthread_t	t;

	philo = (t_philo *)p;
	if ((philo->id & 1) == 0)
		ft_usleep(philo->info->eat / 10);
	while (!is_dead(philo, 0))
	{
		pthread_create(&t, NULL, check_death, p);
		take_fork(philo);
		philo_eat(philo);
		pthread_detach(t);
		if (philo->meal_count == philo->info->eat_count)
		{
			pthread_mutex_lock(&philo->info->m_stop);
			if (++philo->info->philo_eat == philo->info->philo_count)
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

/*
*	Init each philo in the info struct
*	Create thread for each philo and join to main thread
*/
int	init(t_data *info)
{
	int	i;

	info->start_time = timestamp();
	i = -1;
	while (++i < info->philo_count)
	{
		info->philo[i].id = i + 1;
		info->philo[i].last_meal = 0;
		info->philo[i].fork_r = NULL;
		info->philo[i].info = info;
		info->philo[i].meal_count = 0;
		pthread_mutex_init(&(info->philo[i].fork_l), NULL);
		if (i == info->philo_count - 1)
			info->philo[i].fork_r = &info->philo[0].fork_l;
		else
			info->philo[i].fork_r = &info->philo[i + 1].fork_l;
		if (pthread_create(&info->philo[i].thread, NULL, \
				&life, &(info->philo[i])) != 0)
			return (1);
	}
	i = -1;
	while (++i < info->philo_count)
		if (!pthread_join(info->philo[i].thread, NULL))
			return (1);
	return (0);
}
