/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:15:03 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/20 11:58:19 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/philo.h"

long long	the_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	fork_mutex_init(pthread_mutex_t *fork, int philo_num)
{
	int	i;

	i = 0;
	while (i < philo_num)
	{
		pthread_mutex_init(&fork[i], NULL);
		i++;
	}
}

static void	info_init(t_info *info)
{
	info->stop = 0;
	pthread_mutex_init(&(info->stop_lock), NULL);
	pthread_mutex_init(&(info->last_meal_lock), NULL);
	pthread_mutex_init(&(info->write_lock), NULL);
}

static void	ar_init(t_args *args, t_info *in, pthread_mutex_t *fork, char **av)
{
	long long	start_time;
	int			i;
	int			num;

	i = 0;
	num = ft_atoi(av[1]);
	start_time = the_time();
	while (i < num)
	{
		if (av[5])
			args[i].must_eat = ft_atoi(av[5]);
		else
			args[i].must_eat = -1;
		args[i].time_to_die = (ft_atoi(av[2]) * 1000);
		args[i].time_to_eat = ft_atoi(av[3]) * 1000;
		args[i].time_to_sleep = ft_atoi(av[4]) * 1000;
		args[i].info = in;
		args[i].forks = fork;
		args[i].id = i;
		args[i].philo_num = num;
		args[i].meal_eaten = 0;
		args[i].last_meal_time = start_time;
		args[i].start_time = start_time;
		i++;
	}
}

t_args	*init_philo_args(pthread_mutex_t *forks, char **argv)
{
	t_args	*args;
	t_info	*info;
	int		philo_num;

	philo_num = ft_atoi(argv[1]);
	args = malloc(sizeof(t_args) * philo_num);
	if (!args)
	{
		printf("malloc error \n");
		return (NULL);
	}
	info = malloc(sizeof(t_info));
	if (!info)
	{
		printf("malloc info error \n");
		free (args);
		return (NULL);
	}
	info_init(info);
	fork_mutex_init(forks, philo_num);
	ar_init(args, info, forks, argv);
	return (args);
}
