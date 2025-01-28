/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:32:02 by carlos-j          #+#    #+#             */
/*   Updated: 2025/01/28 17:19:27 by carlos-j         ###   ########.fr       */
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
			return (1);
	}
	else
		setup->times_to_eat = 0;
	if (error_flag || setup->time_to_die <= 0 || setup->time_to_eat <= 0
		|| setup->time_to_sleep <= 0 || setup->times_to_eat < 0)
	{
		printf("Error. Test only with positive integer numbers.\n");
		return (1);
	}
	return (0);
}

int	init_values(t_setup *setup)
{
	int	i;

	setup->forks = malloc(sizeof(pthread_mutex_t) * setup->nr_philosophers);
	setup->philos = malloc(sizeof(t_philos) * setup->nr_philosophers);
	setup->stop = 0;
	setup->all_eaten = 0;
	setup->is_dead = 0;
	i = 0;
	while (i < setup->nr_philosophers)
	{
		pthread_mutex_init(&setup->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&setup->write_lock, NULL);
	pthread_mutex_init(&setup->stop_lock, NULL);
	return (0);
}

void	init_philos(t_setup *setup)
{
	int	i;

	i = 0;
	while (i < setup->nr_philosophers)
	{
		setup->philos[i].id = i + 1;
		setup->philos[i].meals = 0;
		setup->philos[i].last_meal = 0;
		setup->philos[i].setup = setup;
		setup->philos[i].left_fork = &setup->forks[i];
		setup->philos[i].right_fork = &setup->forks[(i + 1)
			% setup->nr_philosophers];
		pthread_mutex_init(&setup->philos[i].meals_lock, NULL);
		pthread_mutex_init(&setup->philos[i].last_meal_lock, NULL);
		i++;
	}
}
