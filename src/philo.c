/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:27:37 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/04 11:51:40 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/philo.h"


int	main(int argc, char **argv)
{
	if (argc == 5)
	{
		int				philo_num;
		philo_num = ft_atoi(argv[1]);

		pthread_mutex_t	forks[philo_num];
		pthread_t		philosophers[philo_num];
		pthread_t		monitor;
		pthread_mutex_t	write_lock;
		philo_args		*args;

		pthread_mutex_init(&write_lock, NULL);
		args = init_philo_args(forks, &write_lock, philo_num, argv);
		if(!args)
			return (1);
		if (create_philos(philosophers, args) != 0)
		{
			free(args);
			return (1);
		}
		if (pthread_create(&monitor, NULL, monitor_death, args) != 0)
		{
			printf("Error in creating death monitor thread\n");
			free(args);
			return (1);
		}
		wait_for_philos(philosophers, args);
		exit_philos(args);
		//destroy_forks(forks, args);
		//pthread_mutex_destroy(&write_lock);
		free(args);
	}
	else
		printf("Error Input should be num_of_philo time_to_die time_to_eat\n");
	return (0);
}
