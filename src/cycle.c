/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwong <hwong@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 14:53:25 by nnorazma          #+#    #+#             */
/*   Updated: 2023/01/11 01:55:55 by hwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	take_fork(t_philo *philo)
{
	pthread_mutex_lock(&(philo->fork_l));
	output_philo(philo, " has taken a fork\n", GREEN);
	if (philo->info->philo_count == 1)
	{
		ft_usleep(philo->info->death * 2);
		return ;
	}
	pthread_mutex_lock((philo->fork_r));
	output_philo(philo, " has taken a fork\n", GREEN);
}

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

void	*life(void *p)
{
	t_philo		*philo;
	pthread_t	t;

	philo = (t_philo *)p;
	if (philo->id % 2 == 0)
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

int	init(t_data *data)
{
	int	i;

	data->start_time = timestamp();
	i = -1;
	while (++i < data->philo_count)
	{
		data->philo[i].id = i + 1;
		data->philo[i].last_meal = 0;
		data->philo[i].fork_r = NULL;
		data->philo[i].info = data;
		data->philo[i].meal_count = 0;
		pthread_mutex_init(&(data->philo[i].fork_l), NULL);
		if (i == data->philo_count - 1)
			data->philo[i].fork_r = &data->philo[0].fork_l;
		else
			data->philo[i].fork_r = &data->philo[i + 1].fork_l;
		if (pthread_create(&data->philo[i].thread, NULL, \
				&life, &(data->philo[i])) != 0)
			return (-1);
	}
	i = -1;
	while (++i < data->philo_count)
		if (!pthread_join(data->philo[i].thread, NULL))
			return (1);
	return (0);
}
