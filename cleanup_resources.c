/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_resources.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:26:09 by carlos-j          #+#    #+#             */
/*   Updated: 2025/01/22 16:08:58 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup_resources(t_setup *setup)
{
	int	i;

	i = 0;
	while (i < setup->philosophers)
	{
		pthread_mutex_destroy(&setup->forks[i]);
		pthread_mutex_destroy(&setup->philos[i].meals_lock);
		pthread_mutex_destroy(&setup->philos[i].last_meal_lock);
		i++;
	}
	pthread_mutex_destroy(&setup->write_lock);
	pthread_mutex_destroy(&setup->stop_lock);
	pthread_mutex_destroy(&setup->time_lock);
	free(setup->forks);
	free(setup->philos);
}
