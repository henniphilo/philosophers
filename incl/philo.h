/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:28:02 by hwiemann          #+#    #+#             */
/*   Updated: 2024/05/30 15:01:42 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
#define NUM_PHILO 5

# include <unistd.h>
# include <limits.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>

typedef struct {
	int				philo_num;
	int				time_to_eat;
	int				time_to_think;
	int				time_to_sleep;
	int				time_to_die;
	long			last_meal_time;
	int 			id;
	pthread_mutex_t	*forks;
} philo_args;


void		*ft_philo(void *arg);
long long	the_time();
int			create_philos(pthread_t	*philosoph, philo_args *args);
int			wait_for_philos(pthread_t *philosoph, philo_args *args);
void		eat(int philosopher, philo_args *args);
void		think(int philosopher, philo_args *args);
void		sleepy(int philosopher, philo_args *args);
void		fork_mutex_init(pthread_mutex_t *fork, philo_args *args);
void		destroy_forks(pthread_mutex_t *fork, philo_args *args);
philo_args	*init_philo_args(pthread_mutex_t *forks, char **argv);


# endif
