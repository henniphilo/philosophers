/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:27:37 by hwiemann          #+#    #+#             */
/*   Updated: 2024/05/30 11:13:06 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/philo.h"

void	*ft_philo(philo_args *args)  //muss eigentlich void *arg sein
{
	int	id;
//	philo_args	*args;
	pthread_mutex_t *fork;

//	args = (philo_args *)arg;
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
	}
	return (NULL);
}

void	think(int philosopher, philo_args *args)
{
	printf("Philo %d is thinking \n", philosopher);
	usleep((rand() % 100) * args->time_to_think);
}

void	eat(int philosopher, philo_args *args)
{
	printf("Philo %d is eating \n", philosopher);
	usleep((rand() % 100) * args->time_to_eat);
}


int	create_philos(pthread_t	*philosoph, philo_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		args[i].id = i;
		if(pthread_create(&philosoph[i], NULL, ft_philo(args), &args[i]) != 0)
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
	args = malloc(sizeof(philo_args) * NUM_PHILO);
	if (!args)
	{
		printf("malloc error \n");
		return (NULL);
	}
	while (i < NUM_PHILO)
	{
		args[i].forks = forks;
		i++;
	}
	if (argv[1] && argv[2] && argv[3])
	{
		args->philo_num = atoi(argv[1]);
		args->time_to_eat = atoi(argv[2]) * 1000;
		args->time_to_think = atoi(argv[3]) * 1000;
	}
	return (args);
}

int	main(int argc, char **argv)
{
	pthread_mutex_t	forks[NUM_PHILO];
	pthread_t		philosophers[NUM_PHILO];
	philo_args		*args;

	if (argc == 4)
	{
		args = init_philo_args(forks, argv);
		fork_mutex_init(forks, args);
		if(!args)
			return (1);
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
		while (1)
		{
			usleep(1000);
		}
		destroy_forks(forks, args);
		free(args);
	}
	else
	{
		printf("Error Main %s\n", argv[0]);
		return (1);
	}
	return (0);
}
