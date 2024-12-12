/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:11:19 by carlos-j          #+#    #+#             */
/*   Updated: 2024/12/12 16:00:30 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep
								+ [number_of_times_each_philosopher_must_eat]

==============================================================================

number_of_philosophers: The number of philosophers and also the number of forks.

time_to_die (in milliseconds): If a philosopher didn’t start eating time_to_die
milliseconds since the beginning of their last meal or the beginning of the sim-
ulation, they die.

time_to_eat (in milliseconds): The time it takes for a philosopher to eat.
During that time, they will need to hold two forks.

time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.

number_of_times_each_philosopher_must_eat (optional argument): If all
philosophers have eaten at least number_of_times_each_philosopher_must_eat
times, the simulation stops. If not specified, the simulation stops when a
philosopher dies.




==============================================================================

death delay <= 10

Makefile with -pthread

Allowed functions:
memset
printf
malloc
free
write
usleep
gettimeofday
pthread_create
pthread_detach
pthread_join
pthread_mutex_init
pthread_mutex_destroy
pthread_mutex_lock
pthread_mutex_unlock

==============================================================================
*/

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
	i = 0;
	while (i < setup->philosophers)
	{
		pthread_mutex_init(&setup->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&setup->write_lock, NULL);
	return (0);
}

void	init_philos(t_setup *setup)
{
	int	i;

	i = 0;
	while(i < setup->philosophers)
	{
		setup->philos[i].id = i + 1;
		setup->philos[i].meals = 0;
		setup->philos[i].last_meal = 0;
		setup->philos[i].setup = setup;
		setup->philos[i].left_fork = &setup->forks[i];
		setup->philos[i].right_fork = &setup->forks[(i + 1) % setup->philosophers];
		i++;
	}
}

void task(t_setup *setup)
{
/*
thinking = no forks
eating = uses both forks
sleeping = releases both forks after eating

◦ timestamp_in_ms X has taken a fork
◦ timestamp_in_ms X is eating
◦ timestamp_in_ms X is sleeping
◦ timestamp_in_ms X is thinking
◦ timestamp_in_ms X died
*/

	struct timeval	start_time;
	struct timeval	time_passed;
	printf("=====================================================\n");
	gettimeofday(&start_time, NULL);
	printf("Starting time: %ld\n", start_time.tv_sec);
	printf("=====================================================\n");
	//gettimeofday(&time_passed, NULL);
	//setup->elapsed_time = time_passed.tv_usec - start_time.tv_usec;
	//printf("Time passed: %lld ms\n", setup->elapsed_time);

	int i;
	i = 0;
	while (i <= setup->philosophers)
	{
		printf("philos: %d\n", setup->philosophers);
		if (i % 2 == 0)
		{
			while (setup->philos[i].meals <= setup->times_to_eat)
			{
				printf("%lld %d has taken a fork (%p)\n", setup->elapsed_time, setup->philos[i].id, setup->philos[i].left_fork);
				printf("%lld %d has taken a fork (%p)\n", setup->elapsed_time, setup->philos[i].id, setup->philos[i].right_fork);
				//printf("%lld %d has taken a fork\n", setup->elapsed_time, setup->philos[i].id);
				//printf("%lld %d has taken a fork\n", setup->elapsed_time, setup->philos[i].id);
				printf("%lld %d is eating\n", setup->elapsed_time, setup->philos[i].id);
				setup->philos[i].meals++;
				gettimeofday(&time_passed, NULL);
				setup->elapsed_time = time_passed.tv_usec - start_time.tv_usec + setup->time_to_eat + setup->elapsed_time;
			}
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_setup			setup;

	if (init_checks(argc, argv))
		return (0);
	if (init_values(argc, argv, &setup))
		return (0);
	init_philos(&setup);
	/*printf("philosophers: %d\ntime_to_die: %d\ntime_to_eat: %d\ntime_to_sleep: %d\ntimes_to_eat: %d\n", setup.philosophers, setup.time_to_die,
		setup.time_to_eat, setup.time_to_sleep, setup.times_to_eat);*/
	task(&setup);
}
