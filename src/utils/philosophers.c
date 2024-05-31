/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 13:04:25 by hwiemann          #+#    #+#             */
/*   Updated: 2024/05/31 16:42:36 by hwiemann         ###   ########.fr       */
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


