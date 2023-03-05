/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwong <hwong@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 00:41:52 by hwong             #+#    #+#             */
/*   Updated: 2023/03/05 14:41:36 by hwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isdigit(int ch)
{
	if (ch >= '0' && ch <= '9')
		return (1);
	return (0);
}

static int	ft_atoi(const char *s)
{
	int	num;
	int	sign;

	num = 0;
	sign = 1;
	while (*s == 32 || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '-' || *s == '+')
		if (*s++ == '-')
			sign = -sign;
	while (*s >= '0' && *s <= '9')
		num = num * 10 + (*s++ - 48);
	return (num * sign);
}

static int	is_num(char **av)
{
	int	i;
	int	j;

	i = -1;
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
			if (!ft_isdigit(av[i][j]))
				return (1);
	}
	return (0);
}

/*
*	init all mutexes
*	map the av values into the struct
*	malloc philos
*	handle for av[5]
*/
int	parse(t_data *info, char **av)
{
	pthread_mutex_init(&info->print, NULL);
	pthread_mutex_init(&info->m_stop, NULL);
	pthread_mutex_init(&info->m_eat, NULL);
	pthread_mutex_init(&info->dead, NULL);
	info->stop = 0;
	info->philo_eat = 0;
	info->philo_count = ft_atoi(av[1]);
	info->death = ft_atoi(av[2]);
	info->eat = ft_atoi(av[3]);
	info->sleep = ft_atoi(av[4]);
	info->philo = malloc(sizeof(t_philo) * info->philo_count);
	if (!info->philo)
		return (1);
	if (av[5])
	{
		info->eat_count = ft_atoi(av[5]);
		if (info->eat_count == 0)
			return (1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data	info;

	if (ac != 5 && ac != 6)
		return (printf(YELLOW "Incorrect number of arguments provided.\n./philosophers \
			numOfPhilo death eat sleep [stopEatCount]\n" RESET));
	if (is_num(&av[1]) || is_num(&av[2]) || is_num(&av[3]) || is_num(&av[4]))
		return (printf(RED "Non-numeric arguement(s) provided.\n" RESET));
	info.eat_count = 0;
	if (ac == 6)
		if (is_num(&av[5]))
			return (printf(RED "Non-numeric arguement(s) provided.\n" RESET));
	if (parse(&info, av))
	{
		free(info.philo);
		return (0);
	}
	init(&info);
	freeall(&info);
	return (0);
}
