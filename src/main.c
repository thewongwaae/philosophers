/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwong <hwong@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 14:53:15 by nnorazma          #+#    #+#             */
/*   Updated: 2023/01/09 21:29:57 by hwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_num(char **str)
{
	int	i;
	int	j;

	i = -1;
	while (str[++i])
	{
		j = -1;
		while (str[i][++j])
			if (!ft_isdigit(str[i][j]))
				return (0);
	}
	return (1);
}

int	var_init(t_data *data, char **av)
{
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->m_stop, NULL);
	pthread_mutex_init(&data->m_eat, NULL);
	pthread_mutex_init(&data->dead, NULL);
	data->stop = 0;
	data->philo = malloc(sizeof(t_philo) * data->philo_count);
	if (data->philo == NULL)
		return (2);
	if (is_num(av))
	{
		printf(RED "Non-numeric arguement(s) provided.\n" RESET);
		return (1);
	}
	data->philo_eat = 0;
	data->philo_count = ft_atoi(av[1]);
	data->death = ft_atoi(av[2]);
	data->eat = ft_atoi(av[3]);
	data->sleep = ft_atoi(av[4]);
	if (av[5])
		data->eat_count = ft_atoi(av[5]);
	if (av[5] && data->eat_count == 0)
		return (1);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
		return (printf(YELLOW "Incorrect number of arguments provided.\n./philosophers numOfPhilo death eat sleep [stopEatCount]\n" RESET));
	if (var_init(&data, av) == 1)
	{
		free(data.philo);
		return (0);
	}
	init(&data);
	freeall(&data);
}
