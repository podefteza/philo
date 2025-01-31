/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 10:48:30 by carlos-j          #+#    #+#             */
/*   Updated: 2025/01/31 14:55:08 by carlos-j         ###   ########.fr       */
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
		set_stop_flag(setup, 1);
		usleep(1000);
		pthread_mutex_lock(&setup->write_lock);
		printf("%lld %d died\n", current_time, setup->philo[i].id);
		pthread_mutex_unlock(&setup->write_lock);
		return (1);
	}
	pthread_mutex_lock(&setup->philo[i].meals_lock);
	if (setup->times_to_eat > 0
		&& setup->philo[i].meals >= setup->times_to_eat)
		(*total_meals)++;
	pthread_mutex_unlock(&setup->philo[i].meals_lock);
	return (0);
}

int	check_all_philosophers(t_setup *setup, int *total_meals)
{
	int			i;
	long long	last_meal;

	i = 0;
	*total_meals = 0;
	while (i < setup->nr_philosophers)
	{
		if (get_stop_flag(setup))
			return (1);
		if (check_philosopher_status(setup, i, &last_meal, total_meals))
			return (1);
		i++;
	}
	return (0);
}

void	*check_starvation(void *arg)
{
	t_setup	*setup;
	int		total_meals;
	int		check_interval;

	setup = (t_setup *)arg;
	check_interval = (setup->time_to_die / 10) * 1000;
	if (check_interval > 1000)
		check_interval = 1000;
	while (!get_stop_flag(setup) && (setup->nr_philosophers != 1))
	{
		if (check_all_philosophers(setup, &total_meals))
			return (NULL);
		if (setup->times_to_eat > 0 && total_meals == setup->nr_philosophers)
		{
			set_stop_flag(setup, 1);
			usleep(setup->time_to_eat * 1000);
			pthread_mutex_lock(&setup->write_lock);
			printf("All philosophers have eaten %d times\n",
				setup->times_to_eat);
			pthread_mutex_unlock(&setup->write_lock);
			return (NULL);
		}
		usleep(check_interval);
	}
	return (NULL);
}
