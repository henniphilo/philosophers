/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:28:02 by hwiemann          #+#    #+#             */
/*   Updated: 2024/06/20 11:40:39 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <limits.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>

typedef struct s_info {
	int				stop;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	last_meal_lock;
}	t_info;

typedef struct s_args {
	int				must_eat;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	int				id;
	int				philo_num;
	int				meal_eaten;
	long long		start_time;
	long long		last_meal_time;
	t_info			*info;
	pthread_mutex_t	*forks;
}	t_args;

int			ft_atoi(const char *str);
int			create_philos(pthread_t	*philosoph, t_args *args);
int			wait_for_philos(t_args *args, pthread_t *philosophers);
int			stop_check(t_args *args);
int			ft_strcmp(const char *s1, const char *s2);

void		check_must_eat(t_args *args);
void		*ft_philo(void *arg);
void		*monitor_death(void *arg);
void		eat(int philosopher, t_args *args);
void		think(int philosopher, t_args *args);
void		sleepy(int philosopher, t_args *args);
void		pick_fork(int id, int side, pthread_mutex_t *fork, t_args *args);
void		drop_fork(int id, pthread_mutex_t *fork, t_args *args);
void		fork_mutex_init(pthread_mutex_t *fork, int philo_num);
void		destroy_forks(pthread_mutex_t *fork, t_args *args);
void		log_status(t_args *args, int id, const char *status);
void		ft_exit(t_args *args);
void		philo_threads(t_args *args, pthread_t *philosophers);
void		routine(t_args	*args, pthread_mutex_t *fork, int id);

long long	the_time(void);
t_args		*init_philo_args(pthread_mutex_t *forks, char **argv);

#endif
