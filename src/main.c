/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwong <hwong@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 14:53:15 by nnorazma          #+#    #+#             */
/*   Updated: 2023/01/11 01:54:44 by hwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_num(char **av)
{
	int	i;
	int	j;

	i = -1;
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
			if (!ft_isdigit(av[i][j]))
				return (0);
	}
	return (1);
}

/*
*	Initialise mutexes for different tasks/components of the philosophers
*
*	Parse inputs into their respective struct vars and error check
*/
int	parse(t_data *data, char **av)
{
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->m_stop, NULL);
	pthread_mutex_init(&data->m_eat, NULL);
	pthread_mutex_init(&data->dead, NULL);
	data->stop = 0;
	data->philo = malloc(sizeof(t_philo) * data->philo_count);
	if (!data->philo)
		return (1);
	if (!is_num(&av[1]))
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
	{
		data->eat_count = ft_atoi(av[5]);
		if (!data->eat_count)
			return (1);
	}
	return (0);
}

/*
*	Create a 'data' var inheriting the t_data struct. This
*	contains overall information for the entire program.
*	
*	Initialise the data struct
*
*	
*/
int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
		return (printf(YELLOW "Incorrect number of arguments provided.\n./philosophers numOfPhilo death eat sleep [stopEatCount]\n" RESET));
	if (parse(&data, av))
	{
		free(data.philo);
		return (0);
	}
	init(&data);
	freeall(&data);
}
