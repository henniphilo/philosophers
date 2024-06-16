/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:27:37 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/16 21:49:20 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/philo.h"


int	main(int argc, char **argv)
{
	if (argc == 5 || argc == 6)
	{
		int		philo_num;
		philo_num = ft_atoi(argv[1]);

		pthread_mutex_t	*forks;
		pthread_t		*philosophers;
		pthread_mutex_t	write_lock;
		philo_args		*args;

		forks = malloc(sizeof(pthread_mutex_t) * philo_num);
		philosophers = malloc(sizeof(pthread_t) * philo_num);
		if (!forks || !philosophers)
		{
			printf("Malloc Error \n");
			return (1);
		}
		args = init_philo_args(forks, &write_lock, argv);
		if(!args)
			return (1);
		for (int i = 0; i < philo_num; i++)
		{
			args[i].philosophers = philosophers;
		}
	//	init_philosophers(args, philosophers);
		philo_threads(args);
		ft_exit(args);
	}
	else
		printf("enter [num_of_philo][time_to_die][time_to_eat]([must_eat])\n");
	return (0);
}

void	init_philosophers(philo_args *args, pthread_t *philosophers)
{
	int		i;

	i = 0;
	while (i < args->philo_num)
	{
		args[i].philosophers = philosophers;
	}
}


void		philo_threads(philo_args *args)
{
	pthread_t		monitor;

	if (pthread_create(&monitor, NULL, monitor_death, args) != 0)
	{
		printf("Error in creating death monitor thread\n");
		free(args);
		return ;
	}
	if (create_philos(args->philosophers, args) != 0)
	{
		free(args);
		return ;
	}
	pthread_join(monitor, NULL);
	//wait_for_philos(philosophers, args);
}

void	ft_exit(philo_args *args)
{
	int	i;
	// int	test_a;
	// int	test_b;

	i = 0;
	args->stop = 1;
	wait_for_philos(args); //philosopher join
	destroy_forks(args->forks, args);
	pthread_mutex_destroy(args->write_lock);
	while (i < args->philo_num)
	{
		// test_a = pthread_mutex_trylock(&args[i].meal_check_lock);
		// if (test_a == 0)
		pthread_mutex_unlock(&args[i].meal_check_lock);
		pthread_mutex_destroy(&args[i].meal_check_lock);
		// test_b = pthread_mutex_trylock(&args[i].last_meal_lock);
		// if (test_b == 0)
		pthread_mutex_unlock(&args[i].last_meal_lock);
		pthread_mutex_destroy(&args[i].last_meal_lock);
		i++;
	}
	if (args->forks)
	{
		free(args->forks);
		args->forks = NULL;
	}
	free(args->philosophers);
	free(args);
	exit (0);
}
