/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:46:03 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/20 11:57:48 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/philo.h"

static void	death_case(t_args *args, int i)
{
	log_status(&args[i], i, "died");
	pthread_mutex_unlock(&args->info->last_meal_lock);
	pthread_mutex_lock(&args->info->stop_lock);
	args->info->stop = 1;
	pthread_mutex_unlock(&args->info->stop_lock);
}

void	*monitor_death(void *arg)
{
	int		i;
	t_args	*args;

	args = (t_args *)arg;
	while (1)
	{
		i = 0;
		while (i < args->philo_num)
		{
			if (stop_check(args) == 1)
				return (NULL);
			pthread_mutex_lock(&args->info->last_meal_lock);
			if ((1000 * (the_time() - args[i].last_meal_time))
				>= args->time_to_die)
			{
				death_case(args, i);
				return (NULL);
			}
			pthread_mutex_unlock(&args->info->last_meal_lock);
			i++;
		}
		check_must_eat(args);
		usleep(1000);
	}
	return (NULL);
}

int	stop_check(t_args *args)
{
	pthread_mutex_lock(&args->info->stop_lock);
	if (args->info->stop == 1)
	{
		pthread_mutex_unlock(&args->info->stop_lock);
		return (1);
	}
	pthread_mutex_unlock(&args->info->stop_lock);
	return (0);
}

static int	check_meals(t_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		pthread_mutex_lock(&args->info->last_meal_lock);
		if (args[i].meal_eaten < args->must_eat)
		{
			pthread_mutex_unlock(&args->info->last_meal_lock);
			return (0);
		}
		pthread_mutex_unlock(&args->info->last_meal_lock);
		i++;
	}
	return (1);
}

void	check_must_eat(t_args *args)
{
	int	i;

	i = 0;
	if (args->must_eat > 0)
	{
		if (check_meals(args))
		{
			pthread_mutex_lock(&args->info->stop_lock);
			args->info->stop = 1;
			pthread_mutex_unlock(&args->info->stop_lock);
		}
	}
}
