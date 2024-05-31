/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 13:04:25 by hwiemann          #+#    #+#             */
/*   Updated: 2024/05/31 19:06:39 by hwiemann         ###   ########.fr       */
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
		//check if dead
		think(id, args);
		if (id % 2 == 0)
		{
			pthread_mutex_lock(&fork[id]);
			log_status(args, id, "has taken right fork");
			pthread_mutex_lock(&fork[(id + 1) % args->philo_num]);
			log_status(args, id, "has taken left fork");
		}
		else
		{
			pthread_mutex_lock(&fork[(id + 1) % args->philo_num]);
			log_status(args, id, "has taken right fork");
			pthread_mutex_lock(&fork[id]);
			log_status(args, id, "has taken left fork");
		}
		eat(id, args);
		pthread_mutex_unlock(&fork[id]);
		pthread_mutex_unlock(&fork[(id + 1) % args->philo_num]);
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
		//	printf("the time %lld last meal %ld and time to die %d\n", the_time(), args[i].last_meal_time, args[i].time_to_die);
		//	printf("to die or not to die %lld  time to die is %d\n", (the_time() - args[i].last_meal_time), args[i].time_to_die);
			if((1000 * (the_time() - args[i].last_meal_time)) > args[i].time_to_die) // need to do it on main threat
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
