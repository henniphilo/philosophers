/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:27:37 by hwiemann          #+#    #+#             */
/*   Updated: 2024/05/30 15:32:38 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/philo.h"

void	*ft_philo (void *arg)    //(philo_args *args)  //muss eigentlich void *arg sein
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
			pthread_mutex_lock(&fork[(id + 1) % args->philo_num]);
		}
		else
		{
			pthread_mutex_lock(&fork[(id + 1) % args->philo_num]);
			pthread_mutex_lock(&fork[id]);
		}
		eat(id, args);
		pthread_mutex_unlock(&fork[id]);
		pthread_mutex_unlock(&fork[(id + 1) % args->philo_num]);
		sleepy(id, args);
		if((the_time() - args->last_meal_time) > args->time_to_die)
		{
			printf("Philo %d died\n", id);
			break ;
		}
	}
	return (NULL);
}

long long	the_time()
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	think(int philosopher, philo_args *args)
{
	printf("Philo %d is thinking \n", philosopher);
	usleep((rand() % 100) * args->time_to_think);
}

void	eat(int philosopher, philo_args *args)
{
	printf("Philo %d is eating \n", philosopher);
	args->last_meal_time = the_time();
	usleep((rand() % 100) * args->time_to_eat);
}

void	sleepy(int philosopher, philo_args *args)
{
	printf("Philo %d is sleeping\n", philosopher);
	usleep((rand() % 100) * args->time_to_sleep);
}

int	create_philos(pthread_t	*philosoph, philo_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		args[i].id = i;
		if(pthread_create(&philosoph[i], NULL, ft_philo, &args[i]) != 0)
		{
			printf("Error in creating threads\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	wait_for_philos(pthread_t *philosoph, philo_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		if(pthread_detach(philosoph[i]) != 0)
		{
			printf("Error in detaching threads\n");
			return (1);
		}
		i++;
	}
	return (0);
}

void	fork_mutex_init(pthread_mutex_t *fork, philo_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		pthread_mutex_init(&fork[i], NULL);
		i++;
	}
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

philo_args	*init_philo_args(pthread_mutex_t *forks, char **argv)
{
	int			i;
	philo_args	*args;

	i = 0;
	args = malloc(sizeof(philo_args) * (atoi(argv[1])));
	if (!args)
	{
		printf("malloc error \n");
		return (NULL);
	}
	while (i < atoi(argv[1]))
	{
		args[i].forks = forks;
		i++;
	}
	if (argv[2] && argv[3])
	{
		args->philo_num = atoi(argv[1]);
		args->time_to_eat = atoi(argv[2]) * 1000;
		args->time_to_think = atoi(argv[3]) * 1000;
		args->time_to_sleep = atoi(argv[4]) * 1000;
		args->time_to_die = atoi(argv[5]) * 1000;
	}
	return (args);
}

int	main(int argc, char **argv)
{
	if (argc == 6)
	{
		pthread_mutex_t	forks[atoi(argv[1])];
		pthread_t		philosophers[atoi(argv[1])];
		philo_args		*args;

		args = init_philo_args(forks, argv);
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
		free(args);
	}
	else
		printf("Error with Arguments order should be num_of_philo time_to_eat time_to_think time_to_die\n");
	return (0);
}
