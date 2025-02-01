/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:32:02 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/01 13:02:01 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_args(int argc, char **argv, t_setup *setup)
{
	int	error_flag;

	error_flag = 0;
	setup->nr_philosophers = ft_atoi(argv[1], &error_flag);
	setup->time_to_die = ft_atoi(argv[2], &error_flag);
	setup->time_to_eat = ft_atoi(argv[3], &error_flag);
	setup->time_to_sleep = ft_atoi(argv[4], &error_flag);
	if (argc == 6)
	{
		setup->times_to_eat = ft_atoi(argv[5], &error_flag);
		if (setup->times_to_eat == 0 && !error_flag)
			return (STOP_SIMULATION);
	}
	else
		setup->times_to_eat = 0;
	if (error_flag || setup->time_to_die <= 0 || setup->time_to_eat <= 0
		|| setup->time_to_sleep <= 0 || setup->times_to_eat < 0)
	{
		printf("Error. Test only with positive integer numbers.\n");
		return (STOP_SIMULATION);
	}
	return (CONTINUE_SIMULATION);
}

void	init_values(t_setup *setup)
{
	int	i;

	setup->forks = malloc(sizeof(pthread_mutex_t) * setup->nr_philosophers);
	setup->philo = malloc(sizeof(t_philo) * setup->nr_philosophers);
	setup->stop = 0;
	setup->all_eaten = 0;
	i = 0;
	while (i < setup->nr_philosophers)
	{
		pthread_mutex_init(&setup->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&setup->write_lock, NULL);
	pthread_mutex_init(&setup->stop_lock, NULL);
	gettimeofday(&setup->start_time, NULL);
}

void	init_philos(t_setup *setup)
{
	int	i;

	i = 0;
	while (i < setup->nr_philosophers)
	{
		setup->philo[i].id = i + 1;
		setup->philo[i].meals = 0;
		setup->philo[i].last_meal = 0;
		setup->philo[i].setup = setup;
		setup->philo[i].left_fork = &setup->forks[i];
		setup->philo[i].right_fork = &setup->forks[(i + 1)
			% setup->nr_philosophers];
		pthread_mutex_init(&setup->philo[i].meals_lock, NULL);
		pthread_mutex_init(&setup->philo[i].last_meal_lock, NULL);
		i++;
	}
}
