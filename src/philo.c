/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:27:37 by hwiemann          #+#    #+#             */
/*   Updated: 2024/05/31 11:16:55 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/philo.h"

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
		eat(id, args);
		pthread_mutex_unlock(&fork[id]);
		pthread_mutex_unlock(&fork[(id + 1) % args->philo_num]);
		sleepy(id, args);
		if((the_time() - args->last_meal_time) > args->time_to_die)
		{
			log_status(args, id, "died");
			break ;
		}
		//noch vergleichem meals needed meals eaten?
	}
	return (NULL);
}


int	wait_for_philos(pthread_t *philosoph, philo_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
	//	printf("we are waiting for %d of %d \n", i, args->philo_num);
		if(pthread_join(philosoph[i], NULL) != 0)
		{
			printf("Error in joining threads\n");
			return (1);
		}
		i++;
	}
	return (0);
}

void	destroy_forks(pthread_mutex_t *fork, philo_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		pthread_mutex_destroy(&fork[i]);
		i++;
	}
}

int	main(int argc, char **argv)
{
	if (argc == 6)
	{
		int				philo_num;
		philo_num = atoi(argv[1]);

		pthread_mutex_t	forks[philo_num];
		pthread_t		philosophers[philo_num];
		pthread_mutex_t	write_lock;
		philo_args		*args;

		printf("lets start\n");
		pthread_mutex_init(&write_lock, NULL);
		args = init_philo_args(forks, &write_lock, argv);
		if(!args)
			return (1);
		fork_mutex_init(forks, args);
		if (create_philos(philosophers, args) != 0)
		{
			free(args);
			return (1);
		}
		if (wait_for_philos(philosophers, args))
		{
			free(args);
			return (1);
		}
		destroy_forks(forks, args);
		pthread_mutex_destroy(&write_lock);
		free(args);
	}
	else
		printf("Error with Arguments order should be num_of_philo time_to_eat time_to_think time_to_die\n");
	return (0);
}
