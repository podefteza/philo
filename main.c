/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:11:19 by carlos-j          #+#    #+#             */
/*   Updated: 2025/01/22 10:44:12 by carlos-j         ###   ########.fr       */
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

EAT > SLEEP > THINK


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

TODO:
- test with large arguments for times

- move functions to separate files


https://github.com/dantonik/42-philosophers-tester

https://github.com/MichelleJiam/LazyPhilosophersTester

*/

#include "philo.h"

long long	get_timestamp(struct timeval start_time)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec - start_time.tv_sec) * 1000
		+ (current_time.tv_usec - start_time.tv_usec) / 1000);
}

void	set_stop_flag(t_setup *setup, int value)
{
	pthread_mutex_lock(&setup->stop_lock);
	setup->stop = value;
	pthread_mutex_unlock(&setup->stop_lock);
}

int	get_stop_flag(t_setup *setup)
{
	int	stop;

	pthread_mutex_lock(&setup->stop_lock);
	stop = setup->stop;
	pthread_mutex_unlock(&setup->stop_lock);
	return (stop);
}

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
	printf("%lld %d is thinking\n", get_timestamp(philo->setup->start_time),
		philo->id);
	pthread_mutex_unlock(&philo->setup->write_lock);
	usleep(500);
	return (!get_stop_flag(philo->setup));
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
	if (philo->setup->philosophers == 1)
	{
		one_philo(philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		delay_with_stop_check(philo->setup, philo->setup->time_to_eat / 2);
	while (!get_stop_flag(philo->setup))
	{
		if (!take_forks(philo) || !philo_eating(philo) || !philo_sleeping(philo)
			|| !philo_thinking(philo))
			break ;
	}
	return (NULL);
}

int	check_all_philosophers_eaten(t_setup *setup)
{
	int	i;
	int	total_meals;

	total_meals = 0;
	i = 0;
	while (i < setup->philosophers)
	{
		pthread_mutex_lock(&setup->philos[i].meals_lock);
		if (setup->philos[i].meals >= setup->times_to_eat)
			total_meals++;
		pthread_mutex_unlock(&setup->philos[i].meals_lock);
		i++;
	}
	if (total_meals == setup->philosophers)
	{
		set_stop_flag(setup, 1);
		setup->all_eaten = 1;
		return (1);
	}
	return (0);
}

int	check_philosopher_status(t_setup *setup, int i, long long *last_meal,
		int *total_meals)
{
	long long	current_time;

	pthread_mutex_lock(&setup->philos[i].last_meal_lock);
	*last_meal = setup->philos[i].last_meal;
	pthread_mutex_unlock(&setup->philos[i].last_meal_lock);
	current_time = get_timestamp(setup->start_time);
	if ((current_time - *last_meal) >= setup->time_to_die)
	{
		set_stop_flag(setup, 1);
		pthread_mutex_lock(&setup->write_lock);
		printf("%lld %d died\n", current_time, setup->philos[i].id);
		pthread_mutex_unlock(&setup->write_lock);
		return (1);
	}
	pthread_mutex_lock(&setup->philos[i].meals_lock);
	if (setup->times_to_eat > 0
		&& setup->philos[i].meals >= setup->times_to_eat)
		(*total_meals)++;
	pthread_mutex_unlock(&setup->philos[i].meals_lock);
	return (0);
}

int	check_all_philosophers(t_setup *setup, int *total_meals)
{
	int			i;
	long long	last_meal;

	i = 0;
	*total_meals = 0;
	while (i < setup->philosophers)
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
	while (!get_stop_flag(setup) && (setup->philosophers != 1))
	{
		if (check_all_philosophers(setup, &total_meals))
			return (NULL);
		if (setup->times_to_eat > 0 && total_meals == setup->philosophers)
		{
			set_stop_flag(setup, 1);
			usleep(setup->time_to_eat * 1000);
			pthread_mutex_lock(&setup->write_lock);
			printf("%lld All philosophers have eaten %d times\n",
				get_timestamp(setup->start_time), setup->times_to_eat);
			pthread_mutex_unlock(&setup->write_lock);
			return (NULL);
		}
		usleep(check_interval);
	}
	return (NULL);
}

int	thread_creation(t_setup *setup, pthread_t *philosophers, pthread_t *monitor)
{
	int	i;

	i = 0;
	while (i < setup->philosophers)
	{
		if (pthread_create(&philosophers[i], NULL, philosopher_routine,
				&setup->philos[i]))
		{
			free(philosophers);
			return (1);
		}
		i++;
	}
	if (pthread_create(monitor, NULL, check_starvation, setup))
	{
		free(philosophers);
		return (1);
	}
	return (0);
}

int	task(t_setup *setup)
{
	pthread_t	*philosophers;
	pthread_t	monitor;
	int			i;

	philosophers = malloc(sizeof(pthread_t) * setup->philosophers);
	if (!philosophers)
		return (1);
	if (thread_creation(setup, philosophers, &monitor))
		return (1);
	i = 0;
	while (i < setup->philosophers)
	{
		pthread_join(philosophers[i], NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	free(philosophers);
	return (0);
}

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

void	print_meals_summary(t_setup *setup)
{
	int	i;

	i = 0;
	printf("\n========== Simulation ended ==========\n");
	while (i < setup->philosophers)
	{
		printf("Philosopher %d ate %d times\n", setup->philos[i].id,
			setup->philos[i].meals);
		i++;
	}
	printf("========== Meals summary ==========\n");
}

int	main(int argc, char **argv)
{
	t_setup	setup;

	if (init_checks(argc, argv))
		return (0);
	if (init_args(argc, argv, &setup))
		return (0);
	if (init_values(&setup))
		return (0);
	init_philos(&setup);
	gettimeofday(&setup.start_time, NULL);
	if (task(&setup))
		return (1);
	// print_meals_summary(&setup);
	cleanup_resources(&setup);
	return (0);
}
