/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:27:37 by hwiemann          #+#    #+#             */
/*   Updated: 2024/05/28 19:38:05 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/philo.h"

void	*ft_philo(void *arg)
{
	// char	*str;

	// str = (char *)arg;
	// printf("%s\n", str);

	int	id;
	philo_args	*args;
	pthread_mutex_t *fork;

	args = (philo_args *)arg;
	id = args->id;
	fork = args->forks;

	while (1)
	{
		think(id);
		if (id % 2 == 0)
		{
			pthread_mutex_lock(&fork[id]);
			pthread_mutex_lock(&fork[(id + 1) % NUM_PHILO]);
		}
		else
		{
			pthread_mutex_lock(&fork[(id + 1) % NUM_PHILO]);
			pthread_mutex_lock(&fork[id]);
		}
		eat(id);
		pthread_mutex_unlock(&fork[id]);
		pthread_mutex_unlock(&fork[(id + 1) % NUM_PHILO]);
	}
	return (NULL);
}

void	think(int philosopher)
{
	printf("Philo %d is thinking \n", philosopher);
	usleep((rand() % 100) * 1000);
}

void	eat(int philosopher)
{
	printf("Philo %d is eating \n", philosopher);
	usleep((rand() % 100) * 1000);
}


int	create_philos(pthread_t	*philosoph, philo_args *args)
{
	int	i;

	i = 0;
	while (i < NUM_PHILO)
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

int	wait_for_philos(pthread_t *philosoph)
{
	int	i;

	i = 0;
	while (i < NUM_PHILO)
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

void	fork_mutex_init(pthread_mutex_t *fork)
{
	int	i;

	i = 0;
	while (i < NUM_PHILO)
	{
		pthread_mutex_init(&fork[i], NULL);
		i++;
	}
}

void	destroy_forks(pthread_mutex_t *fork)
{
	int	i;

	i = 0;
	while (i < NUM_PHILO)
	{
		pthread_mutex_destroy(&fork[i]);
		i++;
	}
}

philo_args	*init_philo_args(pthread_mutex_t *forks)
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
	return (args);
}

int	main(int argc, char **argv)
{
	pthread_mutex_t	forks[NUM_PHILO];
	pthread_t		philosophers[NUM_PHILO];
	philo_args		*args;

	if (argc == 2) //muss 3 zum testen 2
	{
		fork_mutex_init(forks);
		args = init_philo_args(forks);
		if(!args)
			return (1);
		if (create_philos(philosophers, args) != 0)
		{
			free(args);
			return (1);
		}
		if (wait_for_philos(philosophers))
		{
			free(args);
			return (1);
		}
		while (1)
		{
			usleep(1000);
		}
		destroy_forks(forks);
		free(args);
	}
	else
	{
		printf("Error Main %s\n", argv[0]);
		return (1);
	}
	return (0);
}
