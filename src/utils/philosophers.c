/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 13:04:25 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/18 18:19:08 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/philo.h"

// left fork has same index as philo , right chopstick is id + 1

void	pick_up_fork (int id, int side, pthread_mutex_t *fork, philo_args *args)
{
	if (side == 0)
	{
		pthread_mutex_lock(&fork[id]); // takes left
		log_status(args, id, "has taken a fork");
	}
	else
	{
		// takes right
		pthread_mutex_lock(&fork[(id + 1) % args->philo_num]); // % num makes it a round table
		log_status(args, id, "has taken a fork");
	}
}

void	drop_down_fork (int id, pthread_mutex_t *fork, philo_args *args)
{
	pthread_mutex_unlock(&fork[id]); // puts down left
	pthread_mutex_unlock(&fork[(id + 1) % args->philo_num]); // % num makes it a round table
	//check_must_eat(args);
}

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
			think(id, args);
			if (id % 2 == 0)
			{
				// if(stop_check(args) == 1)
				// {
				// 	pthread_mutex_lock(&args->write_lock);
				// 	printf("stop ist gerade %d\n", args->stop);
				// 	pthread_mutex_unlock(&args->write_lock);
				// }
				pick_up_fork(id, 0, fork, args);
				pick_up_fork(id, 1, fork, args);
			}
			else
			 {
			// 	if(stop_check(args) == 1)
			// 	{
			// 		pthread_mutex_lock(&args->write_lock);
			// 		printf("stop ist gerade %d\n", args->stop);
			// 		pthread_mutex_unlock(&args->write_lock);
			// 	}
				pick_up_fork(id, 1, fork, args);
				pick_up_fork(id, 0, fork, args);
			}
			eat(id, args);
			sleepy(id, args);
		}
		else
			return (NULL);
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
			if(stop_check(args) == 1)
			{
				// pthread_mutex_lock(&args->info->write_lock);
				// printf(">stop_check exit< stop ist gerade %d\n", args->info->stop);
				// pthread_mutex_unlock(&args->info->write_lock);
				ft_exit(args);
			}
			pthread_mutex_lock(&args->info->last_meal_lock);
			if((1000 * (the_time() - args[i].last_meal_time)) >= args->time_to_die)
			{
				log_status(&args[i], i, "died");
				pthread_mutex_unlock(&args->info->last_meal_lock);
				pthread_mutex_lock(&args->info->stop_lock);
				args->info->stop = 1;
				pthread_mutex_unlock(&args->info->stop_lock);
				// pthread_mutex_lock(&args->info->write_lock);
				// printf(">tot< stop ist gerade %d\n", args->info->stop);
				// pthread_mutex_unlock(&args->info->write_lock);
				ft_exit(args);
				//break ;
			}
			pthread_mutex_unlock(&args->info->last_meal_lock);
			i++;
		}
		check_must_eat(args); // verursacht mehr probleme
		usleep(1000); //to sleep 1ms before checking again
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
