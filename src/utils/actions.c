/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:13:52 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/16 12:07:42 by hwiemann         ###   ########.fr       */
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
//	pthread_mutex_lock(&args->forks[philosopher]);
	log_status(args, philosopher, "is eating");
	args->last_meal_time = the_time();
	pthread_mutex_lock(&args->meal_check_lock);
	args->meal_eaten += 1;
	pthread_mutex_unlock(&args->meal_check_lock);
	usleep(args->time_to_eat);
//	pthread_mutex_unlock(&args->forks[philosopher]);
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

	i = 0;
	if (args->must_eat > 0)
	{
		meals_eaten = 1;
		while (i < args->philo_num)
		{
			pthread_mutex_lock(&args[i].meal_check_lock);
			if (args[i].meal_eaten < args->must_eat)
			{
				meals_eaten = 0;
				// pthread_mutex_lock(args->write_lock);
				// printf(">>philo %d: %d meals eaten of %d \n", i, args[i].meal_eaten, args->must_eat);
				// pthread_mutex_unlock(args->write_lock);
			}
			pthread_mutex_unlock(&args[i].meal_check_lock);
			if (!meals_eaten)
				break ;
			i++;
		}
		if (meals_eaten)
			ft_exit(args);
	}
}
