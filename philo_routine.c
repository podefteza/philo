/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 10:56:49 by carlos-j          #+#    #+#             */
/*   Updated: 2025/01/28 17:21:19 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	one_philo(t_philos *philo)
{
	pthread_mutex_lock(&philo->setup->write_lock);
	printf("%lld %d has taken a fork\n",
		get_timestamp(philo->setup->start_time), philo->id);
	pthread_mutex_unlock(&philo->setup->write_lock);
	usleep(philo->setup->time_to_die * 1000);
	set_stop_flag(philo->setup, 1);
	pthread_mutex_lock(&philo->setup->write_lock);
	printf("%lld %d died\n", get_timestamp(philo->setup->start_time),
		philo->id);
	pthread_mutex_unlock(&philo->setup->write_lock);
}

void	delay_with_stop_check(t_setup *setup, long long delay_ms)
{
	long long	end_time;

	end_time = get_timestamp(setup->start_time) + delay_ms;
	while (!get_stop_flag(setup))
	{
		if (get_timestamp(setup->start_time) >= end_time)
			break ;
		usleep(1000);
	}
}

void	*philosopher_routine(void *arg)
{
	t_philos	*philo;

	philo = (t_philos *)arg;
	pthread_mutex_lock(&philo->last_meal_lock);
	philo->last_meal = get_timestamp(philo->setup->start_time);
	pthread_mutex_unlock(&philo->last_meal_lock);

	if (philo->setup->nr_philosophers == 1)
	{
		one_philo(philo);
		return (NULL);
	}

	if (philo->id % 2 == 0)
		delay_with_stop_check(philo->setup, philo->setup->time_to_eat / 2);

	while (!get_stop_flag(philo->setup))
	{
		if (!take_forks(philo) || !philo_eating(philo) || !philo_sleeping(philo) || !philo_thinking(philo))
			break;
	}
	return (NULL);
}

int	thread_creation(t_setup *setup, pthread_t *monitor)
{
	int	i;

	i = 0;
	while (i < setup->nr_philosophers)
	{
		if (pthread_create(&setup->philos[i].thread, NULL, philosopher_routine,
				&setup->philos[i]))
			return (1);
		i++;
	}
	if (pthread_create(monitor, NULL, check_starvation, setup))
		return (1);
	return (0);
}

int	task(t_setup *setup)
{
	pthread_t	monitor;
	int			i;

	if (thread_creation(setup, &monitor))
		return (1);
	i = 0;
	while (i < setup->nr_philosophers)
	{
		pthread_join(setup->philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	return (0);
}
