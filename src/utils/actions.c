/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:13:52 by hwiemann          #+#    #+#             */
/*   Updated: 2024/05/31 16:57:20 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/philo.h"

void	think(int philosopher, philo_args *args)
{
	log_status(args, philosopher, "is thinking");
	usleep(args->time_to_think);
}

void	eat(int philosopher, philo_args *args)
{
	log_status(args, philosopher, "is eating");
	args->last_meal_time = the_time();
	usleep(args->time_to_eat);
	//args->meals_eaten[philosopher]++;
}

void	sleepy(int philosopher, philo_args *args)
{
	log_status(args, philosopher, "is sleeping");
	usleep(args->time_to_sleep);
}

