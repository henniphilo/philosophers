/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:13:52 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/15 19:09:03 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/philo.h"

void	think(int philosopher, philo_args *args)
{
	log_status(args, philosopher, "is thinking");
	//usleep(args->time_to_think);
}

void	eat(int philosopher, philo_args *args)
{
	log_status(args, philosopher, "is eating");
	args->last_meal_time = the_time();
	args->meal_eaten += 1;
	usleep(args->time_to_eat);
	check_must_eat(args);
}

void	sleepy(int philosopher, philo_args *args)
{
	log_status(args, philosopher, "is sleeping");
	//log_status(args, philosopher, "lays down forks");
	usleep(args->time_to_sleep);
}

void	check_must_eat(philo_args *args)
{
	int		i;
	int		meals_eaten;

	i = 1;
	meals_eaten = args[0].meal_eaten;
	if (args->must_eat > 0)
	{
		while (i < args->philo_num)
		{
			pthread_mutex_lock(args->write_lock);
			printf(">>philo %d: %d meals eaten of %d \n", i, args[i].meal_eaten, args->must_eat);
			pthread_mutex_unlock(args->write_lock);
			if (args[i].meal_eaten != meals_eaten)
				return ;
			i++;
		}
		if (meals_eaten == args->must_eat)
		{
			ft_exit(args);
		}
	}
}
