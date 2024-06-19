/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:20:55 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/19 15:47:07 by hwiemann         ###   ########.fr       */
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


int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			break ;
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	log_status(philo_args *args, int id, const char *status)
{
	long long	time;
	static int	flag = 0;

	pthread_mutex_lock(&args->info->write_lock);
	time = the_time() - args->start_time;
	if (flag == 0)
		printf("%lld %d %s\n", time, id + 1, status);
	if (ft_strcmp(status, "died") == 0)
		flag = 1;
	pthread_mutex_unlock(&args->info->write_lock);
}

int	wait_for_philos(philo_args *args, pthread_t *philosophers)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		if(pthread_join(philosophers[i], NULL) != 0)
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
		if (pthread_mutex_lock(&fork[i]) == 0)
			pthread_mutex_unlock(&fork[i]);
		pthread_mutex_destroy(&fork[i]);
		i++;
	}
}
