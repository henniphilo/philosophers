/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:48:58 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/20 15:50:22 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/philo.h"

static void	free_data(t_args *args)
{
	if (args->info)
	{
		free(args->info);
		args->info = NULL;
	}
	if (args->forks)
	{
		free(args->forks);
		args->forks = NULL;
	}
	free(args);
}

int	free_space(pthread_mutex_t *forks, pthread_t *philos, t_args *args)
{
	if (forks)
		free(forks);
	if (philos)
		free(philos);
	if (args)
		free_data(args);
	return (1);
}

void	ft_exit(t_args *args)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&args->info->stop_lock);
	args->info->stop = 1;
	pthread_mutex_unlock(&args->info->stop_lock);
	if (pthread_mutex_lock(&args->info->last_meal_lock) == 0)
		pthread_mutex_unlock(&args->info->last_meal_lock);
	pthread_mutex_destroy(&args->info->last_meal_lock);
	if (pthread_mutex_lock(&args->info->write_lock) == 0)
		pthread_mutex_unlock(&args->info->write_lock);
	pthread_mutex_destroy(&args->info->write_lock);
	if (pthread_mutex_lock(&args->info->stop_lock) == 0)
		pthread_mutex_unlock(&args->info->stop_lock);
	pthread_mutex_destroy(&args->info->stop_lock);
	destroy_forks(args->forks, args);
	free_data(args);
	exit (0);
}
