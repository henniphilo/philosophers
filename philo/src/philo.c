/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:27:37 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/21 12:01:11 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/philo.h"

static int	valid_input(char **argv)
{
	int	i;

	i = 1;
	while (i < 5)
	{
		if (ft_atoi(argv[i]) == -1)
			return (1);
		i++;
	}
	if (argv[5] && ft_atoi(argv[5]) == -1)
	{
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	pthread_mutex_t	*forks;
	pthread_t		*philosophers;
	t_args			*args;
	int				philo_num;

	if ((argc == 5 || argc == 6) && (valid_input(argv) == 0))
	{
		args = NULL;
		philo_num = ft_atoi(argv[1]);
		if (make_space(&forks, &philosophers, philo_num) != 0)
			return (1);
		args = init_philo_args(forks, argv);
		if (!args)
			return (free_space(forks, philosophers, args));
		if (philo_threads(args, philosophers) == 1)
			return (free_space(forks, philosophers, args));
		free_space(forks, philosophers, args);
		free(philosophers);
		ft_exit(args);
	}
	else
		printf("enter [num_of_philo][time_to_die][time_to_eat][time_to_sleep]([must_eat])\n");
	return (0);
}

int	make_space(pthread_mutex_t **forks, pthread_t **philos, int num)
{
	*forks = malloc(sizeof(pthread_mutex_t) * num);
	*philos = malloc(sizeof(pthread_t) * num);
	if (!*forks || !*philos)
	{
		printf("Malloc Error \n");
		free(*forks);
		free(*philos);
		return (1);
	}
	return (0);
}

int	philo_threads(t_args *args, pthread_t *philosophers)
{
	pthread_t	monitor;

	if (pthread_create(&monitor, NULL, monitor_death, args) != 0)
	{
		printf("Error in creating death monitor thread\n");
		free(args);
		return (1);
	}
	if (create_philos(philosophers, args) != 0)
	{
		free(args);
		return (1);
	}
	pthread_join(monitor, NULL);
	wait_for_philos(args, philosophers);
	free(philosophers);
	ft_exit(args);
	return (0);
}
