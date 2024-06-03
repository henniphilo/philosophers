/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:13:52 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/03 19:00:28 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/philo.h"

void	think(int philosopher, philo_args *args)
{
	log_status(args, philosopher, "is thinking");
}

void	eat(int philosopher, philo_args *args)
{
	log_status(args, philosopher, "is eating");
	pthread_mutex_lock(&args->last_meal_time_lock[philosopher]);
	args->last_meal_time = the_time();
	pthread_mutex_unlock(&args->last_meal_time_lock[philosopher]);
	usleep(args->time_to_eat);
}

void	sleepy(int philosopher, philo_args *args)
{
	log_status(args, philosopher, "is sleeping");
	usleep(args->time_to_sleep);
}

