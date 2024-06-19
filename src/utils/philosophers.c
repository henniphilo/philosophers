/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 13:04:25 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/19 16:54:03 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/philo.h"

// left fork has same index as philo , right chopstick is id + 1
// &fork[(id + 1) % args->philo_num to have round table


void	*ft_philo (void *arg)
{
	int	id;
	philo_args	*args;
	pthread_mutex_t *fork;

	args = (philo_args *)arg;
	id = args->id;
	fork = args->forks;

	while (1)
	{
		if(stop_check(args) != 1)
		{
			routine(args, fork, id);
		}
		else
			return (NULL);
	}
	return (NULL);
}

void	routine(philo_args	*args, pthread_mutex_t *fork, int id)
{
	think(id, args);
	if (args->philo_num == 1)
		return ;
	if (id % 2 == 0)
	{
		pick_up_fork(id, 0, fork, args);
		pick_up_fork(id, 1, fork, args);
	}
	else
	 {
		pick_up_fork(id, 1, fork, args);
		pick_up_fork(id, 0, fork, args);
	}
	eat(id, args);
	sleepy(id, args);
}

static void	death_case(philo_args *args, int i)
{
	log_status(&args[i], i, "died");
	pthread_mutex_unlock(&args->info->last_meal_lock);
	pthread_mutex_lock(&args->info->stop_lock);
	args->info->stop = 1;
	pthread_mutex_unlock(&args->info->stop_lock);
}

void	*monitor_death(void *arg)
{
	philo_args	*args;
	int			i;

	args = (philo_args *)arg;
	while (1)
	{
		i = 0;
		while (i < args->philo_num)
		{
			if(stop_check(args) == 1)
				return (NULL);
			pthread_mutex_lock(&args->info->last_meal_lock);
			if((1000 * (the_time() - args[i].last_meal_time)) >= args->time_to_die)
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


int	stop_check(philo_args *args)
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
