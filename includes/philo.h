/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwong <hwong@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 14:53:28 by nnorazma          #+#    #+#             */
/*   Updated: 2023/01/11 01:45:36 by hwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/time.h>
# include <limits.h>

# define RESET "\033[0m"
# define RED "\033[31m" //death
# define CYAN "\033[36m" //eating
# define YELLOW "\033[38;5;220m" //thinking
# define GREEN "\033[32m" //take fork
# define PURPLE "\033[38;5;129m" //sleeping

# define SUCC 0
# define ERR 1

typedef struct s_philo
{
	int				id;
	int				meal_count;
	bool			is_eating;
	long int		last_meal;
	struct s_data	*info;
	pthread_mutex_t	*fork_r;
	pthread_mutex_t	fork_l;
	pthread_t		thread;
}					t_philo;

typedef struct s_data
{
	int				philo_eat;
	int				philo_count;
	int				death;
	int				eat;
	int				sleep;
	int				eat_count;
	int				stop;
	long int		start_time;
	t_philo			*philo;
	pthread_mutex_t	print;
	pthread_mutex_t	m_stop;
	pthread_mutex_t	m_eat;
	pthread_mutex_t	dead;
}					t_data;

int			init(t_data *data);
void		*life(void *p);
void		philo_eat(t_philo *philo);
void		take_fork(t_philo *philo);
void		*check_death(void *p);

long long	timestamp(void);
void		ft_usleep(int ms);
void		freeall(t_data *data);
int			is_dead(t_philo *philo, int nb);
void		output_philo(t_philo *philo, char *str, char *colour);

int			ft_isdigit(int ch);
int			ft_atoi(const char *s);

#endif
