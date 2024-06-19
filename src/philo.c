/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:27:37 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/19 14:12:59 by hwiemann         ###   ########.fr       */
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
		philo_args		*args;

		forks = malloc(sizeof(pthread_mutex_t) * philo_num);
		philosophers = malloc(sizeof(pthread_t) * philo_num);
		if (!forks || !philosophers)
		{
			printf("Malloc Error \n");
			return (1);
		}
		args = init_philo_args(forks, argv);
		if(!args)
			return (1);
		philo_threads(args, philosophers);
		ft_exit(args);
	}
	else
		printf("enter [num_of_philo][time_to_die][time_to_eat]([must_eat])\n");
	return (0);
}


void		philo_threads(philo_args *args, pthread_t *philosophers)
{
	pthread_t		monitor;

	if (pthread_create(&monitor, NULL, monitor_death, args) != 0)
	{
		printf("Error in creating death monitor thread\n");
		free(args);
		return ;
	}
	if (create_philos(philosophers, args) != 0)
	{
		free(args);
		return ;
	}
	pthread_join(monitor, NULL);
	wait_for_philos(args, philosophers); //philosopher join
}

void	ft_exit(philo_args *args)
{
	int	i;

	i = 0;

	pthread_mutex_lock(&args->info->stop_lock);
	args->info->stop = 1;
	pthread_mutex_unlock(&args->info->stop_lock);
	//wait_for_philos(args, philosophers); //philosopher join
	while (i < args->philo_num)
	{
		if (pthread_mutex_lock(&args->info->last_meal_lock) == 0)
			pthread_mutex_unlock(&args->info->last_meal_lock);
		pthread_mutex_destroy(&args->info->last_meal_lock);
		i++;
	}
	pthread_mutex_destroy(&args->info->write_lock);
	destroy_forks(args->forks, args);
	if (args->forks)
	{
		free(args->forks);
		args->forks = NULL;
	}
//	free(args->philosophers);
	free(args);
	exit (0);
}
