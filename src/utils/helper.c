/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:20:55 by hwiemann          #+#    #+#             */
/*   Updated: 2024/05/31 17:27:35 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/philo.h"

int		ft_atoi(const char *str)
{
	int	sign;
	int	result;
	int	i;

	sign = 1;
	result = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
	{
		i++;
	}
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			sign *= -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}
// logs time and state of philos
// uses mutex to not mix up other output
void	log_status(philo_args *args, int id, const char *status)
{
	pthread_mutex_lock(args->write_lock);
	printf("%lld %d %s\n", the_time(), id + 1, status);
	pthread_mutex_unlock(args->write_lock);
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

void	*monitor_death(void *arg)
{
	philo_args	*args;
	int			i;

	args = (philo_args *)arg;
	while (1)
	{
		i = 0;
		while (i < args->philo_num)
		{
		//	printf("the time %lld last meal %ld and time to die %d\n", the_time(), args[i].last_meal_time, args[i].time_to_die);
		//	printf("to die or not to die %lld  time to die is %d\n", (the_time() - args[i].last_meal_time), args[i].time_to_die);
			if((1000 * (the_time() - args[i].last_meal_time)) > args[i].time_to_die) // need to do it on main threat
			{
				log_status(&args[i], i, "died");
				exit (0);
			}
			i++;
		}
		usleep(1000); //to sleep 1ms before checking again
	}
	return (NULL);
}
