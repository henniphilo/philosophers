/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:13:52 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/19 16:46:37 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/philo.h"

void	think(int philosopher, philo_args *args)
{
	log_status(args, philosopher, "is thinking");
	usleep(1000);
}

void	pick_up_fork (int id, int side, pthread_mutex_t *fork, philo_args *args)
{
	if (side == 0)
	{
		pthread_mutex_lock(&fork[id]);
		log_status(args, id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&fork[(id + 1) % args->philo_num]);
		log_status(args, id, "has taken a fork");
	}
}

void	drop_down_fork (int id, pthread_mutex_t *fork, philo_args *args)
{
	pthread_mutex_unlock(&fork[id]);
	pthread_mutex_unlock(&fork[(id + 1) % args->philo_num]);
}


void	eat(int philosopher, philo_args *args)
{
	pthread_mutex_lock(&args->info->last_meal_lock);
	args->last_meal_time = the_time();
	pthread_mutex_unlock(&args->info->last_meal_lock);
	if (stop_check(args) == 1)
	{
		drop_down_fork(philosopher, args->forks, args);
		return ;
	}
	log_status(args, philosopher, "is eating");
	usleep(args->time_to_eat);
	pthread_mutex_lock(&args->info->last_meal_lock);
	args->meal_eaten += 1;
	pthread_mutex_unlock(&args->info->last_meal_lock);
	drop_down_fork(philosopher, args->forks, args);
}

void	sleepy(int philosopher, philo_args *args)
{
	if (stop_check(args) == 1)
			return ;
	log_status(args, philosopher, "is sleeping");
	usleep(args->time_to_sleep);
}

void	check_must_eat(philo_args *args)
{
	int		i;
	int		meals_eaten;

	i = 0;
	if (args->must_eat > 0)
	{
		meals_eaten = 1;
		while (i < args->philo_num)
		{
			pthread_mutex_lock(&args->info->last_meal_lock);
			if (args[i].meal_eaten < args->must_eat)
			{
				meals_eaten = 0;
				pthread_mutex_unlock(&args->info->last_meal_lock);
				break ;
			}
			pthread_mutex_unlock(&args->info->last_meal_lock);
			i++;
		}
		if (meals_eaten)
		{
			pthread_mutex_lock(&args->info->stop_lock);
			args->info->stop = 1;
			pthread_mutex_unlock(&args->info->stop_lock);
		}
	}
}
