/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:11:19 by carlos-j          #+#    #+#             */
/*   Updated: 2024/12/17 15:21:44 by carlos-j         ###   ########.fr       */
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
- edge case for 1 philo > Philosopher 1 takes a fork and dies after time_to_die

- after all philos ate,	the program keeps running for a while instead of quitting
should wait for the time_to_eat of that last philo,	and then show the message and quit.

- program to stop working when a philo dies


https://github.com/dantonik/42-philosophers-tester

*/

#include "philo.h"

long long	get_timestamp(struct timeval start_time)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec - start_time.tv_sec) * 1000
		+ (current_time.tv_usec - start_time.tv_usec) / 1000);
}

void	*philosopher_routine(void *arg)
{
	t_philos	*philo;

	philo = (t_philos *)arg;
	if (philo->setup->stop == 1)
			return (NULL);

	if (philo->id % 2 == 0)
		usleep(philo->setup->time_to_eat * 1000 / 2);
	//while (1) // or setup->stop != 1
	while (philo->setup->stop != 1)
	{
		if (philo->setup->stop == 1)
			return (NULL);
		pthread_mutex_lock(&philo->setup->stop_lock);
		if (philo->setup->stop == 1)
		{
			pthread_mutex_unlock(&philo->setup->stop_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->setup->stop_lock);
		pthread_mutex_lock(&philo->setup->stop_lock);
		philo->setup->elapsed_time = get_timestamp(philo->setup->start_time);
		pthread_mutex_unlock(&philo->setup->stop_lock);
		pthread_mutex_lock(philo->left_fork);
		if (philo->setup->stop == 1)
		{
			//pthread_mutex_unlock(&philo->setup->stop_lock);
			break ;
		}
		printf("%lld %d has taken a fork\n", philo->setup->elapsed_time,
			philo->id);
		if (philo->setup->philosophers == 1)
		{
			// takes one fork and dies after time_to_die
			usleep(philo->setup->time_to_die);
			philo->setup->stop = 1;
			printf("%lld 1 died\n", philo->setup->elapsed_time +  philo->setup->time_to_die);
			break;
		}
		if (philo->setup->stop == 1)
		{
			//pthread_mutex_unlock(&philo->setup->stop_lock);
			break ;
		}
		pthread_mutex_lock(philo->right_fork);
		printf("%lld %d has taken a fork\n", philo->setup->elapsed_time,
			philo->id);

		// Eating phase
		if (philo->setup->stop == 1)
		{
			//pthread_mutex_unlock(&philo->setup->stop_lock);
			break ;
		}
		pthread_mutex_lock(&philo->setup->stop_lock);
		philo->last_meal = philo->setup->elapsed_time;
		pthread_mutex_unlock(&philo->setup->stop_lock);
		philo->meals++;
		printf("%lld %d is eating\n", philo->setup->elapsed_time, philo->id);
		usleep(philo->setup->time_to_eat * 1000);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);

		if (philo->setup->stop == 1)
		{
			return (NULL);
		}

		// Sleeping phase
		pthread_mutex_lock(&philo->setup->stop_lock);
		philo->setup->elapsed_time = get_timestamp(philo->setup->start_time);
		pthread_mutex_unlock(&philo->setup->stop_lock);
		printf("%lld %d is sleeping\n", philo->setup->elapsed_time, philo->id);
		usleep(philo->setup->time_to_sleep * 1000);

		if (philo->setup->stop == 1)
		{
			return (NULL);
		}

		// Thinking phase
		pthread_mutex_lock(&philo->setup->stop_lock);
		philo->setup->elapsed_time = get_timestamp(philo->setup->start_time);
		pthread_mutex_unlock(&philo->setup->stop_lock);
		printf("%lld %d is thinking\n", philo->setup->elapsed_time, philo->id);
	}
	return (NULL);
}

void	*check_starvation(void *arg)
{
	t_setup		*setup;
	int			i;
	long long	last_meal;
	int			all_eaten;

	setup = (t_setup *)arg;
	while (setup->stop != 1)
	{
		pthread_mutex_lock(&setup->stop_lock);
		setup->elapsed_time = get_timestamp(setup->start_time);
		pthread_mutex_unlock(&setup->stop_lock);
		i = 0;
		while (i < setup->philosophers)
		{
			pthread_mutex_lock(&setup->stop_lock);
			last_meal = setup->philos[i].last_meal;
			pthread_mutex_unlock(&setup->stop_lock);
			if (setup->philos[i].meals > 0 || setup->philosophers == 1)
			{
				if ((setup->elapsed_time - last_meal) > setup->time_to_die)
				{
					pthread_mutex_lock(&setup->stop_lock);
					printf("%lld %d died\n", setup->elapsed_time,
						setup->philos[i].id);
					setup->stop = 1;
					pthread_mutex_unlock(&setup->stop_lock);
					return (NULL);
				}
			}
			i++;
			if (setup->stop == 1)
				break;
		}
		if (setup->times_to_eat > 0)
		{
			all_eaten = 1;
			i = 0;
			while (i < setup->philosophers)
			{
				if (setup->philos[i].meals < setup->times_to_eat)
				{
					all_eaten = 0;
					break ;
				}
				i++;
			}
			if (all_eaten)
			{
				usleep(setup->time_to_eat);
				pthread_mutex_lock(&setup->stop_lock);
				printf("%lld ", setup->elapsed_time + setup->time_to_eat);
				printf("All philosophers have eaten %d times\n",
					setup->times_to_eat);
				setup->stop = 1;
				pthread_mutex_unlock(&setup->stop_lock);
				return (NULL);
			}
		}
		usleep(1000);
	}
	return (NULL);
}

int	task(t_setup *setup)
{
	pthread_t	monitor_thread;
	int			i;

	if (pthread_create(&monitor_thread, NULL, check_starvation, setup) != 0)
	{
		printf("Error: Failed to create monitoring thread\n");
		return (1);
	}
	i = 0;
	while (i < setup->philosophers)
	{
		if (pthread_create(&setup->philos[i].thread, NULL, philosopher_routine,
				&setup->philos[i]) != 0)
		{
			pthread_mutex_lock(&setup->stop_lock);
			setup->stop = 1;
			pthread_mutex_unlock(&setup->stop_lock);
			break ;
		}
		i++;
	}
	i = 0;
	while (i < setup->philosophers)
	{
		pthread_mutex_lock(&setup->stop_lock);
		if (setup->stop == 1)
		{
			pthread_mutex_unlock(&setup->stop_lock);
			break ;
		}
		pthread_mutex_unlock(&setup->stop_lock);
		pthread_join(setup->philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	return (0);
}

int	main(int argc, char **argv)
{
	t_setup	setup;
	int		i;

	i = 0;
	if (init_checks(argc, argv))
		return (0);
	if (init_values(argc, argv, &setup))
		return (0);
	init_philos(&setup);
	gettimeofday(&setup.start_time, NULL);
	if (task(&setup) == 1)
		return (0);

	// DEBUG ============ DELETE LATER...
	/*printf("======== Simulation ended ========\n");
	while (i < setup.philosophers)
	{
		printf("Philosopher %d ate %d times\n", setup.philos[i].id,
			setup.philos[i].meals);
		i++;
	}
	printf("======== Meals summary ========\n");*/
	i = 0;
	while (i < setup.philosophers)
	{
		pthread_mutex_destroy(&setup.forks[i]);
		i++;
	}
	pthread_mutex_destroy(&setup.write_lock);
	pthread_mutex_destroy(&setup.stop_lock);
	free(setup.forks);
	free(setup.philos);
	return (0);
}
