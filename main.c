/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:11:19 by carlos-j          #+#    #+#             */
/*   Updated: 2025/01/15 09:06:14 by carlos-j         ###   ########.fr       */
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
- add debug mode:
	- show nr of meals eaten by each philo after each meal
	- print summary

- verify if makefile can contain tsanitize tag


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

static int take_forks(t_philos *philo)
{
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;
    int stop;
    long long timestamp;

    if (philo->id % 2 == 0) {
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    } else {
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    }

    pthread_mutex_lock(&philo->setup->stop_lock);
    stop = philo->setup->stop;
    pthread_mutex_unlock(&philo->setup->stop_lock);
    if (stop)
        return (0);

    pthread_mutex_lock(first_fork);

    // Check stop again before printing
    pthread_mutex_lock(&philo->setup->stop_lock);
    stop = philo->setup->stop;
    pthread_mutex_unlock(&philo->setup->stop_lock);
    if (stop)
    {
        pthread_mutex_unlock(first_fork);
        return (0);
    }

    timestamp = get_timestamp(philo->setup->start_time);
    pthread_mutex_lock(&philo->setup->write_lock);
    printf("%lld %d has taken a fork\n", timestamp, philo->id);
    pthread_mutex_unlock(&philo->setup->write_lock);

    pthread_mutex_lock(&philo->setup->stop_lock);
    stop = philo->setup->stop;
    pthread_mutex_unlock(&philo->setup->stop_lock);
    if (stop)
    {
        pthread_mutex_unlock(first_fork);
        return (0);
    }

    pthread_mutex_lock(second_fork);

    // Check stop before final print
    pthread_mutex_lock(&philo->setup->stop_lock);
    stop = philo->setup->stop;
    pthread_mutex_unlock(&philo->setup->stop_lock);
    if (stop)
    {
        pthread_mutex_unlock(first_fork);
        pthread_mutex_unlock(second_fork);
        return (0);
    }

    timestamp = get_timestamp(philo->setup->start_time);
    pthread_mutex_lock(&philo->setup->write_lock);
    printf("%lld %d has taken a fork\n", timestamp, philo->id);
    pthread_mutex_unlock(&philo->setup->write_lock);

    return (1);
}

static void release_forks(t_philos *philo)
{
    if (philo->id % 2 == 0) {
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);
    } else {
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
    }
}

static void think_time(t_philos *philo)
{
    long long time_to_think;

    // Calculate optimal thinking time
    time_to_think = (philo->setup->time_to_die -
                    (philo->setup->time_to_eat + philo->setup->time_to_sleep)) / 2;

    if (time_to_think > 0)
        usleep(time_to_think * 1000);
}

void *philosopher_routine(void *arg)
{
    t_philos *philo;
    int stop;

    philo = (t_philos *)arg;

	pthread_mutex_lock(&philo->last_meal_lock);
    philo->last_meal = get_timestamp(philo->setup->start_time);
    pthread_mutex_unlock(&philo->last_meal_lock);

    // Handle single philosopher case
	if (philo->setup->philosophers == 1)
{
    pthread_mutex_lock(&philo->setup->write_lock);
    printf("%lld %d has taken a fork\n", get_timestamp(philo->setup->start_time), philo->id);
    pthread_mutex_unlock(&philo->setup->write_lock);

    usleep(philo->setup->time_to_die * 1000); // Wait for philosopher to die

    // Only print death message if we haven't already stopped
    pthread_mutex_lock(&philo->setup->stop_lock);
    if (!philo->setup->stop)
    {
        philo->setup->stop = 1;
        pthread_mutex_unlock(&philo->setup->stop_lock);

        pthread_mutex_lock(&philo->setup->write_lock);
        printf("%lld %d died\n", get_timestamp(philo->setup->start_time), philo->id);
        pthread_mutex_unlock(&philo->setup->write_lock);
    }
    else
    {
        pthread_mutex_unlock(&philo->setup->stop_lock);
    }

    // Don't try to unlock the fork since we never locked it
    return (NULL);
}

     if (philo->id % 2 == 0)
        usleep((philo->setup->time_to_eat / 2) * 1000);

    while (1)
    {
        // Check if thinking time is needed
        long long current_time = get_timestamp(philo->setup->start_time);
        pthread_mutex_lock(&philo->last_meal_lock);
        long long time_since_meal = current_time - philo->last_meal;
        pthread_mutex_unlock(&philo->last_meal_lock);

		// If we have plenty of time before death, wait a bit to let others eat
        if (time_since_meal < (philo->setup->time_to_die / 2))
            usleep(1000); // Small delay to prevent resource contention


		// Check stop condition before each major action
        pthread_mutex_lock(&philo->setup->stop_lock);
        stop = philo->setup->stop;
        pthread_mutex_unlock(&philo->setup->stop_lock);
        if (stop)
            return (NULL);

        if (!take_forks(philo))
            return (NULL);

        // Check stop condition after taking forks
        pthread_mutex_lock(&philo->setup->stop_lock);
        stop = philo->setup->stop;
        pthread_mutex_unlock(&philo->setup->stop_lock);
        if (stop)
        {
            release_forks(philo);
            return (NULL);
        }

        // Eating phase
        pthread_mutex_lock(&philo->last_meal_lock);
        philo->last_meal = get_timestamp(philo->setup->start_time);
        pthread_mutex_unlock(&philo->last_meal_lock);

        pthread_mutex_lock(&philo->meals_lock);
        philo->meals++;
        pthread_mutex_unlock(&philo->meals_lock);

        pthread_mutex_lock(&philo->setup->write_lock);
        printf("%lld %d is eating\n", get_timestamp(philo->setup->start_time), philo->id);
        pthread_mutex_unlock(&philo->setup->write_lock);

        usleep(philo->setup->time_to_eat * 1000);
        release_forks(philo);

        // Check stop before sleeping
        pthread_mutex_lock(&philo->setup->stop_lock);
        stop = philo->setup->stop;
        pthread_mutex_unlock(&philo->setup->stop_lock);
        if (stop)
            return (NULL);

        pthread_mutex_lock(&philo->setup->write_lock);
        printf("%lld %d is sleeping\n", get_timestamp(philo->setup->start_time), philo->id);
        pthread_mutex_unlock(&philo->setup->write_lock);
        usleep(philo->setup->time_to_sleep * 1000);

        // Check stop before thinking
        pthread_mutex_lock(&philo->setup->stop_lock);
        stop = philo->setup->stop;
        pthread_mutex_unlock(&philo->setup->stop_lock);
        if (stop)
            return (NULL);

        // Thinking phase with calculated delay
        pthread_mutex_lock(&philo->setup->write_lock);
        printf("%lld %d is thinking\n", get_timestamp(philo->setup->start_time), philo->id);
        pthread_mutex_unlock(&philo->setup->write_lock);

        think_time(philo);
    }
    return (NULL);
}

void *check_starvation(void *arg)
{
    t_setup *setup = (t_setup *)arg;
    int i;
    long long last_meal;
    long long current_time;
    int total_meals;
    int stop;
	int check_interval;

	// Calculate optimal check interval
    check_interval = (setup->time_to_die / 10) * 1000; // 10% of time_to_die
    if (check_interval > 1000)
        check_interval = 1000; // Cap at 1ms for responsiveness

    while (1)
    {
        total_meals = 0;
        for (i = 0; i < setup->philosophers; i++)
        {
            pthread_mutex_lock(&setup->stop_lock);
            stop = setup->stop;
            pthread_mutex_unlock(&setup->stop_lock);
            if (stop)
                return (NULL);

            pthread_mutex_lock(&setup->philos[i].last_meal_lock);
            last_meal = setup->philos[i].last_meal;
            pthread_mutex_unlock(&setup->philos[i].last_meal_lock);

            current_time = get_timestamp(setup->start_time);

            // Check if philosopher has started eating yet
            if (last_meal == 0)
                last_meal = current_time;

            if ((current_time - last_meal) >= setup->time_to_die)
            {
                pthread_mutex_lock(&setup->stop_lock);
                setup->stop = 1;
                pthread_mutex_unlock(&setup->stop_lock);

                pthread_mutex_lock(&setup->write_lock);
                printf("%lld %d died\n", current_time, setup->philos[i].id);
                pthread_mutex_unlock(&setup->write_lock);
                return (NULL);
            }

            pthread_mutex_lock(&setup->philos[i].meals_lock);
            if (setup->times_to_eat > 0 && setup->philos[i].meals >= setup->times_to_eat)
                total_meals++;
            pthread_mutex_unlock(&setup->philos[i].meals_lock);
        }

        if (setup->times_to_eat > 0 && total_meals == setup->philosophers)
        {
            pthread_mutex_lock(&setup->stop_lock);
            setup->stop = 1;
            setup->all_eaten = 1;
            pthread_mutex_unlock(&setup->stop_lock);
            return (NULL);
        }
        usleep(check_interval);
    }
    return (NULL);
}

int task(t_setup *setup)
{
    pthread_t *philosophers;
    pthread_t monitor;
    int i;

    philosophers = malloc(sizeof(pthread_t) * setup->philosophers);
    if (!philosophers)
        return (1);

    // Create philosopher threads
    for (i = 0; i < setup->philosophers; i++)
    {
        if (pthread_create(&philosophers[i], NULL, philosopher_routine, &setup->philos[i]))
        {
            free(philosophers);
            return (1);
        }
    }

    // Create monitor thread
    if (pthread_create(&monitor, NULL, check_starvation, setup))
    {
        free(philosophers);
        return (1);
    }

    // Join philosopher threads
    for (i = 0; i < setup->philosophers; i++)
        pthread_join(philosophers[i], NULL);

    // Join monitor thread
    pthread_join(monitor, NULL);

    free(philosophers);
    return (0);
}

void	cleanup_resources(t_setup *setup)
{
	int i;

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
	int i;

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
	if (init_values(argc, argv, &setup))
		return (0);
	init_philos(&setup);
	gettimeofday(&setup.start_time, NULL);

	if (task(&setup))
        return (1);
	pthread_mutex_lock(&setup.time_lock);
	setup.elapsed_time = get_timestamp(setup.start_time);
	pthread_mutex_unlock(&setup.time_lock);
	if (setup.all_eaten == 1)
	{
		printf("%lld All philosophers have eaten %d times\n",
			setup.elapsed_time, setup.times_to_eat);
	}
	#ifdef DEBUG
		print_meals_summary(&setup);
	#endif
	cleanup_resources(&setup);
	return (0);
}
