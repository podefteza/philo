/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 10:48:30 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/02 16:14:28 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philosopher_status(t_setup *setup, int i, long long *last_meal,
		int *total_meals)
{
	long long	current_time;

	pthread_mutex_lock(&setup->philo[i].last_meal_lock);
	*last_meal = setup->philo[i].last_meal;
	pthread_mutex_unlock(&setup->philo[i].last_meal_lock);
	current_time = get_timestamp(setup->start_time);
	if ((current_time - *last_meal) >= setup->time_to_die)
	{
		set_stop_flag(setup, STOP_SIMULATION);
		usleep(1000);
		pthread_mutex_lock(&setup->write_lock);
		printf("%lld %d died\n", current_time, setup->philo[i].id);
		pthread_mutex_unlock(&setup->write_lock);
		return (STOP_SIMULATION);
	}
	pthread_mutex_lock(&setup->philo[i].meals_lock);
	if (setup->times_to_eat > 0
		&& setup->philo[i].meals >= setup->times_to_eat)
		(*total_meals)++;
	pthread_mutex_unlock(&setup->philo[i].meals_lock);
	return (CONTINUE_SIMULATION);
}

int	check_all_philosophers(t_setup *setup, int *total_meals)
{
	int			i;
	long long	last_meal;

	i = 0;
	*total_meals = 0;
	while (i < setup->nr_philosophers)
	{
		if (get_stop_flag(setup) == STOP_SIMULATION)
			return (STOP_SIMULATION);
		if (check_philosopher_status(setup, i, &last_meal, total_meals))
			return (STOP_SIMULATION);
		i++;
	}
	return (CONTINUE_SIMULATION);
}

void	*all_philosophers_eaten(t_setup *setup)
{
	set_stop_flag(setup, STOP_SIMULATION);
	usleep(setup->time_to_eat * 1000);
	pthread_mutex_lock(&setup->write_lock);
	printf("%lld All philosophers have eaten %d times\n",
		get_timestamp(setup->start_time), setup->times_to_eat);
	pthread_mutex_unlock(&setup->write_lock);
	return (NULL);
}

void	*check_starvation(void *arg)
{
	t_setup	*setup;
	int		philos_full;
	int		check_interval;

	setup = (t_setup *)arg;
	check_interval = (setup->time_to_die / 10) * 1000;
	if (check_interval > 1000)
		check_interval = 1000;
	while (!get_stop_flag(setup) && (setup->nr_philosophers != 1))
	{
		philos_full = 0;
		if (check_all_philosophers(setup, &philos_full))
			return (NULL);
		if (setup->times_to_eat > 0 && philos_full == setup->nr_philosophers)
			return (all_philosophers_eaten(setup));
		usleep(check_interval);
	}
	return (NULL);
}
