/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 15:25:26 by carlos-j          #+#    #+#             */
/*   Updated: 2025/01/22 09:12:30 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	log_fork_taken(t_philos *philo)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->setup->write_lock);
	timestamp = get_timestamp(philo->setup->start_time);
	printf("%lld %d has taken a fork\n", timestamp, philo->id);
	pthread_mutex_unlock(&philo->setup->write_lock);
}

static int	take_first_fork(t_philos *philo, pthread_mutex_t *first_fork)
{
	pthread_mutex_lock(first_fork);
	if (get_stop_flag(philo->setup))
	{
		pthread_mutex_unlock(first_fork);
		return (0);
	}
	log_fork_taken(philo);
	return (1);
}

static int	take_second_fork(t_philos *philo, pthread_mutex_t *first_fork,
		pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(second_fork);
	if (get_stop_flag(philo->setup))
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return (0);
	}
	log_fork_taken(philo);
	return (1);
}

int	take_forks(t_philos *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->id % 2 == 0)
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	else
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	if (!take_first_fork(philo, first_fork))
		return (0);
	if (!take_second_fork(philo, first_fork, second_fork))
		return (0);
	return (1);
}

void	release_forks(t_philos *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}
