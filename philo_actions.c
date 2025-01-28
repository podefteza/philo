/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 10:44:00 by carlos-j          #+#    #+#             */
/*   Updated: 2025/01/28 17:16:41 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_eating(t_philos *philo)
{
	long long	eat_end;
	long long	current_time;

	pthread_mutex_lock(&philo->last_meal_lock);
	philo->last_meal = get_timestamp(philo->setup->start_time);
	pthread_mutex_unlock(&philo->last_meal_lock);
	pthread_mutex_lock(&philo->meals_lock);
	philo->meals++;
	pthread_mutex_unlock(&philo->meals_lock);
	pthread_mutex_lock(&philo->setup->write_lock);
	if (!get_stop_flag(philo->setup))
		printf("%lld %d is eating\n", get_timestamp(philo->setup->start_time),
			philo->id);
	pthread_mutex_unlock(&philo->setup->write_lock);
	eat_end = get_timestamp(philo->setup->start_time)
		+ philo->setup->time_to_eat;
	while (!get_stop_flag(philo->setup))
	{
		current_time = get_timestamp(philo->setup->start_time);
		if (current_time >= eat_end)
			break ;
		usleep(1000);
	}
	release_forks(philo);
	return (!get_stop_flag(philo->setup));
}

int	philo_sleeping(t_philos *philo)
{
	long long	sleep_end;
	long long	current_time;

	pthread_mutex_lock(&philo->setup->write_lock);
	if (!get_stop_flag(philo->setup))
		printf("%lld %d is sleeping\n", get_timestamp(philo->setup->start_time),
		philo->id);
	pthread_mutex_unlock(&philo->setup->write_lock);
	sleep_end = get_timestamp(philo->setup->start_time)
		+ philo->setup->time_to_sleep;
	while (!get_stop_flag(philo->setup))
	{
		current_time = get_timestamp(philo->setup->start_time);
		if (current_time >= sleep_end)
			break ;
		usleep(1000);
	}
	return (!get_stop_flag(philo->setup));
}

int	philo_thinking(t_philos *philo)
{
	pthread_mutex_lock(&philo->setup->write_lock);
	if (!get_stop_flag(philo->setup))
		printf("%lld %d is thinking\n", get_timestamp(philo->setup->start_time), philo->id);
	pthread_mutex_unlock(&philo->setup->write_lock);

	usleep(500);
	return (!get_stop_flag(philo->setup));
}
