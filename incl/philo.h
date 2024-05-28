/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:28:02 by hwiemann          #+#    #+#             */
/*   Updated: 2024/05/28 19:33:54 by hwiemann         ###   ########.fr       */
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
	int 			id;
	pthread_mutex_t	*forks;
} philo_args;


void	*ft_philo(void *arg);
int		create_philos(pthread_t	*philosoph, philo_args *args);
int		wait_for_philos(pthread_t *philosoph);
void	eat(int philosopher);
void	think(int philosopher);
void	fork_mutex_init(pthread_mutex_t *fork);
void	destroy_forks(pthread_mutex_t *fork);
philo_args	*init_philo_args(pthread_mutex_t *forks);


# endif
