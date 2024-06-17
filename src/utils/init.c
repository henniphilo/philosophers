/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:15:03 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/17 12:59:50 by hwiemann         ###   ########.fr       */
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
	int			philo_num;
	int			*fin_meal;
	long long	start_time;

	i = 0;
	philo_num = ft_atoi(argv[1]);
	args = malloc(sizeof(philo_args) * philo_num);
	fin_meal = malloc(sizeof(int) * philo_num);
	if (!args || !fin_meal)
	{
		printf("malloc error \n");
		return (NULL);
	}
	while (i < philo_num)
	{
		fin_meal[i] = 0;
		i++;
	}
	i = 0;
	fork_mutex_init(forks, write_lock, philo_num);
	start_time = the_time();
	while (i < philo_num)
	{
		args[i].stop = 0;
		args[i].fin_meal = fin_meal;
		args[i].forks = forks;
		args[i].write_lock = *write_lock;
		args[i].id = i;
		args[i].philo_num = philo_num;
		args[i].time_to_die = (ft_atoi(argv[2]) * 1000);
		args[i].time_to_eat = ft_atoi(argv[3]) * 1000;
		args[i].time_to_sleep = ft_atoi(argv[4]) * 1000;
		args[i].meal_eaten = 0;
		args[i].last_meal_time = start_time;
		args[i].start_time = start_time;
		if (argv[5])
			args[i].must_eat = ft_atoi(argv[5]);
		else
			args[i].must_eat = -1;
		pthread_mutex_init(&args[i].meal_check_lock, NULL);
		pthread_mutex_init(&args[i].fin_meal_lock, NULL);
		pthread_mutex_init(&args[i].last_meal_lock, NULL);
		i++;
	}
	return (args);
}

