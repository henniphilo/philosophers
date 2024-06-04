/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 13:04:25 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/04 15:14:05 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/philo.h"

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
		think(id, args);
		if (id % 2 == 0)
		{
			pthread_mutex_lock(&fork[id]);
			log_status(args, id, "has taken a fork");
			pthread_mutex_lock(&fork[(id + 1) % args->philo_num]);
			log_status(args, id, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(&fork[(id + 1) % args->philo_num]);
			log_status(args, id, "has taken a fork");
			pthread_mutex_lock(&fork[id]);
			log_status(args, id, "has taken a fork");
		}
		pthread_mutex_unlock(&fork[id]);
		pthread_mutex_unlock(&fork[(id + 1) % args->philo_num]);
		eat(id, args);
		sleepy(id, args);
	}
	return (NULL);
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
			if((1000 * (the_time() - args[i].last_meal_time)) > args[i].time_to_die)
			{
				log_status(&args[i], i, "died");
				exit (1);
			}
			i++;
		}
		usleep(1000); //to sleep 1ms before checking again
	}
	return (NULL);
}
