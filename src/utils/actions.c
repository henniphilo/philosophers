/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:13:52 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/14 17:49:13 by hwiemann         ###   ########.fr       */
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
	usleep(args->time_to_eat);
}

void	sleepy(int philosopher, philo_args *args)
{
	log_status(args, philosopher, "is sleeping");
	//log_status(args, philosopher, "lays down forks");
	usleep(args->time_to_sleep);
}

