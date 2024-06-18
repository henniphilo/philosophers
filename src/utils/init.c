/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:15:03 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/18 15:48:33 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/philo.h"

long long	the_time()
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	fork_mutex_init(pthread_mutex_t *fork, pthread_mutex_t *write_lock, int philo_num)
{
	int	i;

	i = 0;
	while (i < philo_num)
	{
		pthread_mutex_init(&fork[i], NULL);
		i++;
	}
	pthread_mutex_init(write_lock, NULL);
}

int	create_philos(pthread_t	*philosoph, philo_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		args[i].id = i;
		if(pthread_create(&philosoph[i], NULL, ft_philo, &args[i]) != 0)
		{
			printf("Error in creating threads\n");
			return (1);
		}
		i++;
	}
	return (0);
}

philo_args	*init_philo_args(pthread_mutex_t *forks, pthread_mutex_t *write_lock, char **argv)
{
	int			i;
	philo_args	*args;
	philo_info	*info;
	int			philo_num;
	long long	start_time;

	i = 0;
	philo_num = ft_atoi(argv[1]);
	args = malloc(sizeof(philo_args) * philo_num);
	if (!args)
	{
		printf("malloc error \n");
		return (NULL);
	}
	info = malloc(sizeof(philo_info));
	if (!info)
	{
		printf("malloc info error \n");
		free (args);
		return (NULL);
	}
	info->stop = 0;
	start_time = the_time();
	info->write_lock = *write_lock;
	pthread_mutex_init(&(info->stop_lock), NULL);
	fork_mutex_init(forks, write_lock, philo_num);
	i = 0;
	while (i < philo_num)
	{
		if (argv[5])
			args[i].must_eat = ft_atoi(argv[5]);
		else
			args[i].must_eat = -1;
		args[i].time_to_die = (ft_atoi(argv[2]) * 1000);
		args[i].time_to_eat = ft_atoi(argv[3]) * 1000;
		args[i].time_to_sleep = ft_atoi(argv[4]) * 1000;
		args[i].info = info;
		args[i].forks = forks;
		args[i].id = i;
		args[i].philo_num = philo_num;
		args[i].meal_eaten = 0;
		args[i].last_meal_time = start_time;
		args[i].start_time = start_time;
		// pthread_mutex_init(&args[i].meal_check_lock, NULL);
		pthread_mutex_init(&args[i].last_meal_lock, NULL);
		i++;
	}
	return (args);
}

