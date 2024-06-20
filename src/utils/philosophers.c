/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 13:04:25 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/20 11:47:30 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/philo.h"

// left fork has same index as philo , right chopstick is id + 1
// &fork[(id + 1) % args->philo_num to have round table

void	*ft_philo(void *arg)
{
	int				id;
	t_args			*args;
	pthread_mutex_t	*fork;

	args = (t_args *)arg;
	id = args->id;
	fork = args->forks;
	while (1)
	{
		if (stop_check(args) != 1)
		{
			routine(args, fork, id);
		}
		else
			return (NULL);
	}
	return (NULL);
}

void	routine(t_args	*args, pthread_mutex_t *fork, int id)
{
	think(id, args);
	if (args->philo_num == 1)
		return ;
	if (id % 2 == 0)
	{
		pick_fork(id, 0, fork, args);
		pick_fork(id, 1, fork, args);
	}
	else
	{
		pick_fork(id, 1, fork, args);
		pick_fork(id, 0, fork, args);
	}
	eat(id, args);
	sleepy(id, args);
}

int	create_philos(pthread_t	*philosoph, t_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		args[i].id = i;
		if (pthread_create(&philosoph[i], NULL, ft_philo, &args[i]) != 0)
		{
			printf("Error in creating threads\n");
			return (1);
		}
		i++;
	}
	return (0);
}
