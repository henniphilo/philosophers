/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:27:37 by hwiemann          #+#    #+#             */
/*   Updated: 2024/05/31 11:03:57 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/philo.h"

long long	the_time()
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	log_status(philo_args *args, int id, const char *status)
{
	pthread_mutex_lock(args->write_lock);
	printf("%lld %d %s\n", the_time(), id + 1, status);
	pthread_mutex_unlock(args->write_lock);
}

void	think(int philosopher, philo_args *args)
{
	log_status(args, philosopher, "is thinking");
	usleep((rand() % 100) * args->time_to_think);
}

void	eat(int philosopher, philo_args *args)
{
	log_status(args, philosopher, "is eating");
	args->last_meal_time = the_time();
	usleep((rand() % 100) * args->time_to_eat);
	//args->meals_eaten[philosopher]++;
}

void	sleepy(int philosopher, philo_args *args)
{
	log_status(args, philosopher, "is sleeping");
	usleep((rand() % 100) * args->time_to_sleep);
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

int	create_philos(pthread_t	*philosoph, philo_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		printf("philo %d of %d exists now \n", i, args->philo_num);
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

void	fork_mutex_init(pthread_mutex_t *fork, philo_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
//		printf("fork %d of %d is init \n", i, args->philo_num);
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

philo_args	*init_philo_args(pthread_mutex_t *forks, pthread_mutex_t *write_lock, char **argv)
{
	int			i;
	philo_args	*args;
	int			philo_num;

	i = 0;
	args = malloc(sizeof(philo_args) * (atoi(argv[1])));
	if (!args)
	{
		printf("malloc error \n");
		return (NULL);
	}
	philo_num = atoi(argv[1]);
	while (i < philo_num)
	{
		args[i].forks = forks;
		args[i].id = i;
		args[i].write_lock = write_lock;
		args[i].philo_num = philo_num;
		args[i].time_to_eat = atoi(argv[2]) * 1000;
		args[i].time_to_think = atoi(argv[3]) * 1000;
		args[i].time_to_sleep = atoi(argv[4]) * 1000;
		args[i].time_to_die = atoi(argv[5]) * 1000;
		args[i].last_meal_time = the_time();
		i++;
	}
	return (args);
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
