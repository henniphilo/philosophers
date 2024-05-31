/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:27:37 by hwiemann          #+#    #+#             */
/*   Updated: 2024/05/31 13:44:42 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/philo.h"


int	main(int argc, char **argv)
{
	if (argc == 6)
	{
		int				philo_num;
		philo_num = ft_atoi(argv[1]);

		pthread_mutex_t	forks[philo_num];
		pthread_t		philosophers[philo_num];
		pthread_mutex_t	write_lock;
		philo_args		*args;

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
		wait_for_philos(philosophers, args);
		destroy_forks(forks, args);
		pthread_mutex_destroy(&write_lock);
		free(args);
	}
	else
		printf("Error with Arguments order should be num_of_philo time_to_die time_to_eat time_to_think\n");
	return (0);
}
