/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:32:02 by carlos-j          #+#    #+#             */
/*   Updated: 2025/01/09 20:31:55 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_checks(int argc, char **argv)
{
	int	i;
	int	args;

	if (argc < 5 || argc > 6)
	{
		printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat");
		printf(" time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	i = 0;
	args = argc - 1;
	while (args > 1)
	{
		while (argv[args][i])
		{
			if (!ft_isdigit(argv[args][i]))
			{
				printf("Error. Test only with positive integer numbers.\n");
				return (1);
			}
			i++;
		}
		args--;
	}
	return (0);
}

int	init_values(int argc, char **argv, t_setup *setup)
{
	int	i;

	setup->philosophers = ft_atoi(argv[1]);
	setup->time_to_die = ft_atoi(argv[2]);
	setup->time_to_eat = ft_atoi(argv[3]);
	setup->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		setup->times_to_eat = ft_atoi(argv[5]);
	else
		setup->times_to_eat = 0;
	if (setup->philosophers <= 0 || setup->time_to_die <= 0
		|| setup->time_to_eat <= 0 || setup->time_to_sleep <= 0
		|| setup->times_to_eat < 0)
	{
		printf("Error. Test only with positive integer numbers.\n");
		return (1);
	}
	setup->forks = malloc(sizeof(pthread_mutex_t) * setup->philosophers);
	setup->philos = malloc(sizeof(t_philos) * setup->philosophers);
	setup->stop = 0;
	setup->all_eaten = 0;
	setup->is_dead = 0;
	i = 0;
	while (i < setup->philosophers)
	{
		pthread_mutex_init(&setup->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&setup->write_lock, NULL);
	pthread_mutex_init(&setup->stop_lock, NULL);
	pthread_mutex_init(&setup->time_lock, NULL);
	return (0);
}

void	init_philos(t_setup *setup)
{
	int	i;

	i = 0;
	while (i < setup->philosophers)
	{
		setup->philos[i].id = i + 1;
		setup->philos[i].meals = 0;
		//setup->philos[i].last_meal = get_timestamp(setup->start_time);
		setup->philos[i].last_meal = 0;
		setup->philos[i].setup = setup;
		setup->philos[i].left_fork = &setup->forks[i];
		setup->philos[i].right_fork = &setup->forks[(i + 1)
			% setup->philosophers];
		pthread_mutex_init(&setup->philos[i].meals_lock, NULL);
		pthread_mutex_init(&setup->philos[i].last_meal_lock, NULL);
		i++;
	}
}
