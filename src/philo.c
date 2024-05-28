/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwiemann <hwiemann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:27:37 by hwiemann          #+#    #+#             */
/*   Updated: 2024/05/28 17:42:21 by hwiemann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/philo.h"

void	*ft_philo(void *arg)
{
	char	*str;

	str = (char *)arg;
	printf("%s\n", str);
	return (NULL);
}

int	create_philos(pthread_t	*thread_id, void *argument, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if(pthread_create(&thread_id[i], NULL, ft_philo, (void*)argument))
		{
			printf("Error in creating threads\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	wait_for_philos(pthread_t *thread_id, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if(pthread_join(thread_id[i], NULL))
		{
			printf("Error in joining threads\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	pthread_t	thread_id[2];

	if (argc == 2) //muss 3 zum testen 2
	{
		if (create_philos(thread_id, argv[1], 2))
			return (1);
		if (wait_for_philos(thread_id, 2))
			return (1);
	}
	else
	{
		printf("Error Main\n");
		return (1);
	}
	return (0);
}
