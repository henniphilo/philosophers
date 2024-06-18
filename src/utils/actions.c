/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:13:52 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/18 14:21:02 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/philo.h"

void	think(int philosopher, philo_args *args)
{
	log_status(args, philosopher, "is thinking");
	usleep(1000);
}

void	eat(int philosopher, philo_args *args)
{
	pthread_mutex_lock(&args->last_meal_lock);
	args->last_meal_time = the_time();
	args->meal_eaten += 1;
	pthread_mutex_unlock(&args->last_meal_lock);
	//pthread_mutex_lock(&args->meal_check_lock);
	//pthread_mutex_unlock(&args->meal_check_lock);
	log_status(args, philosopher, "is eating");
	usleep(args->info->time_to_eat);
	drop_down_fork(philosopher, args->forks, args);
	check_must_eat(args);
}

void	sleepy(int philosopher, philo_args *args)
{
	log_status(args, philosopher, "is sleeping");
	usleep(args->info->time_to_sleep);
}

void	check_must_eat(philo_args *args)
{
	int		i;
	int		meals_eaten;

	i = 0;
	if (args->info->must_eat > 0)
	{
		meals_eaten = 1;
		while (i < args->philo_num)
		{
			//pthread_mutex_lock(&args[i].meal_check_lock);
			pthread_mutex_lock(&args[i].last_meal_lock);
			if (args[i].meal_eaten < args->info->must_eat)
			{
				meals_eaten = 0;
			}
		//	pthread_mutex_unlock(&args[i].meal_check_lock);
			pthread_mutex_unlock(&args[i].last_meal_lock);
			if (!meals_eaten)
				break ;
			i++;
		}
		if (meals_eaten)
		{
			pthread_mutex_lock(&args->stop_lock);
			args->info->stop = 1;
			pthread_mutex_unlock(&args->stop_lock);
		}
			//ft_exit(args);  // put it in monitor to have only place to kill everyone
	}
}
