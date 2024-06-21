/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:13:52 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/21 12:01:24 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/philo.h"

void	think(int philosopher, t_args *args)
{
	log_status(args, philosopher, "is thinking");
	usleep(1000);
}

int	pick_fork(int id, int side, pthread_mutex_t *fork, t_args *args)
{
	if (side == 0)
	{
		pthread_mutex_lock(&fork[id]);
		log_status(args, id, "has taken a fork");
	}
	else
	{
		if (&fork[id] != &fork[(id + 1) % args->philo_num]
			&& (pthread_mutex_lock(&fork[(id + 1) % args->philo_num]) == 0))
			log_status(args, id, "has taken a fork");
		else
			return (1);
	}
	return (0);
}

void	drop_fork(int id, pthread_mutex_t *fork, t_args *args)
{
	pthread_mutex_unlock(&fork[id]);
	pthread_mutex_unlock(&fork[(id + 1) % args->philo_num]);
}

void	eat(int philosopher, t_args *args)
{
	pthread_mutex_lock(&args->info->last_meal_lock);
	args->last_meal_time = the_time();
	pthread_mutex_unlock(&args->info->last_meal_lock);
	if (stop_check(args) == 1)
	{
		drop_fork(philosopher, args->forks, args);
		return ;
	}
	log_status(args, philosopher, "is eating");
	usleep(args->time_to_eat);
	pthread_mutex_lock(&args->info->last_meal_lock);
	args->meal_eaten += 1;
	pthread_mutex_unlock(&args->info->last_meal_lock);
	drop_fork(philosopher, args->forks, args);
}

void	sleepy(int philosopher, t_args *args)
{
	if (stop_check(args) == 1)
		return ;
	log_status(args, philosopher, "is sleeping");
	usleep(args->time_to_sleep);
}
