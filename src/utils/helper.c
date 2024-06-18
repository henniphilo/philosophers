/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:20:55 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/17 17:45:59 by hwiemann         ###   ########.fr       */
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
// uses mutex to not mix up other outputgf
void	log_status(philo_args *args, int id, const char *status)
{
	long long	time;

	pthread_mutex_lock(&args->write_lock);
	time = the_time() - args->start_time;
	printf("%lld %d %s\n", time, id + 1, status);
	pthread_mutex_unlock(&args->write_lock);
}

int	wait_for_philos(philo_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		if(pthread_join(args->philosophers[i], NULL) != 0)
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
//	check_fin_meal(args);
	while (i < args->philo_num)
	{
		if (pthread_mutex_lock(&fork[i]) == 0)
			pthread_mutex_unlock(&fork[i]);
		pthread_mutex_destroy(&fork[i]);
		i++;
	}
}

// void	check_fin_meal(philo_args *args)
// {
// 	int		i;

// 	i = 0;
// 	pthread_mutex_lock(&args->fin_meal_lock);
// 	while (i < args->philo_num)
// 	{
// 		if (args->fin_meal[i] == 1)
// 		{
// 			drop_down_fork(i, args->forks, args);
// 		}
// 		i++;
// 	}
// 	pthread_mutex_unlock(&args->fin_meal_lock);
// }
