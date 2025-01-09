/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:11:19 by carlos-j          #+#    #+#             */
/*   Updated: 2025/01/09 17:24:54 by carlos-j         ###   ########.fr       */
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
    int         stop;

    philo = (t_philos *)arg;

	// Handle single philosopher case
	if (philo->setup->philosophers == 1)
	{
		pthread_mutex_lock(&philo->setup->write_lock);
		printf("%lld %d has taken a fork\n", get_timestamp(philo->setup->start_time), philo->id);
		pthread_mutex_unlock(&philo->setup->write_lock);

		usleep(philo->setup->time_to_die * 1000); // Wait for philosopher to die

		pthread_mutex_lock(&philo->setup->stop_lock);
		philo->setup->stop = 1; // Set the stop flag
		pthread_mutex_unlock(&philo->setup->stop_lock);

		pthread_mutex_lock(&philo->setup->write_lock);
		printf("%lld %d died\n", get_timestamp(philo->setup->start_time), philo->id);
		pthread_mutex_unlock(&philo->setup->write_lock);

		pthread_mutex_unlock(philo->left_fork); // Unlock the fork
		return (NULL); // Exit the thread
	}

    if (philo->id % 2 == 0)
        usleep(philo->setup->time_to_eat * 1000 / 2);

    while (1)
    {
        // Check if simulation should stop
        pthread_mutex_lock(&philo->setup->stop_lock);
        stop = philo->setup->stop;
        pthread_mutex_unlock(&philo->setup->stop_lock);
        if (stop)
            return (NULL);

        // Take first fork
        pthread_mutex_lock(philo->left_fork);

		 // Check stop immediately after taking a fork
        pthread_mutex_lock(&philo->setup->stop_lock);
        stop = philo->setup->stop;
        pthread_mutex_unlock(&philo->setup->stop_lock);
        if (stop)
        {
            pthread_mutex_unlock(philo->left_fork);
            return (NULL);
        }

        pthread_mutex_lock(&philo->setup->write_lock);
        printf("%lld %d has taken a fork\n", get_timestamp(philo->setup->start_time), philo->id);
        pthread_mutex_unlock(&philo->setup->write_lock);

        // Take second fork
        pthread_mutex_lock(philo->right_fork);
        pthread_mutex_lock(&philo->setup->stop_lock);
        stop = philo->setup->stop;
        pthread_mutex_unlock(&philo->setup->stop_lock);
        if (stop)
        {
            pthread_mutex_unlock(philo->right_fork);
            return (NULL);
        }
		pthread_mutex_lock(&philo->setup->write_lock);
        printf("%lld %d has taken a fork\n", get_timestamp(philo->setup->start_time), philo->id);
        pthread_mutex_unlock(&philo->setup->write_lock);

        // Eating
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
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);

        // Check stop before sleeping
        pthread_mutex_lock(&philo->setup->stop_lock);
        stop = philo->setup->stop;
        pthread_mutex_unlock(&philo->setup->stop_lock);
        if (stop)
            return (NULL);

        // Sleeping
        pthread_mutex_lock(&philo->setup->write_lock);
        printf("%lld %d is sleeping\n", get_timestamp(philo->setup->start_time), philo->id);
        pthread_mutex_unlock(&philo->setup->write_lock);
        usleep(philo->setup->time_to_sleep * 1000);

        // Thinking
        pthread_mutex_lock(&philo->setup->stop_lock);
        stop = philo->setup->stop;
        pthread_mutex_unlock(&philo->setup->stop_lock);
        if (stop)
        {
            pthread_mutex_unlock(philo->left_fork);
            return (NULL);
        }
        pthread_mutex_lock(&philo->setup->write_lock);
        printf("%lld %d is thinking\n", get_timestamp(philo->setup->start_time), philo->id);
        pthread_mutex_unlock(&philo->setup->write_lock);
    }
    return (NULL);
}

void	*check_starvation(void *arg)
{
    t_setup		*setup;
    int			i;
    long long	last_meal;
    long long   current_time;
    int         stop;
	int all_eaten = 0;

    setup = (t_setup *)arg;
    while (1)
    {
        // Check if simulation should stop
        pthread_mutex_lock(&setup->stop_lock);
        stop = setup->stop;
        pthread_mutex_unlock(&setup->stop_lock);
        if (stop)
            return (NULL);

        current_time = get_timestamp(setup->start_time);
        i = 0;
        while (i < setup->philosophers)
        {
            // Get last meal time safely
            pthread_mutex_lock(&setup->philos[i].last_meal_lock);
            last_meal = setup->philos[i].last_meal;
			pthread_mutex_unlock(&setup->philos[i].last_meal_lock);
			pthread_mutex_lock(&setup->philos[i].meals_lock);
            int meals = setup->philos[i].meals;
            pthread_mutex_unlock(&setup->philos[i].meals_lock);

            // Check for death
            if (meals > 0)
            {
                if ((current_time - last_meal) > setup->time_to_die)
                {
                    pthread_mutex_lock(&setup->stop_lock);
                    setup->is_dead = i + 1;
                    setup->stop = 1;
                    pthread_mutex_unlock(&setup->stop_lock);
                    pthread_mutex_lock(&setup->write_lock);
                    printf("%lld %d died\n", current_time, setup->philos[i].id);
                    pthread_mutex_unlock(&setup->write_lock);
                    return (NULL);
                }
            }
            i++;
        }
        // Check if all philosophers have eaten enough
        if (setup->times_to_eat > 0)
        {
            all_eaten = 1;
            for (i = 0; i < setup->philosophers; i++)
            {
                pthread_mutex_lock(&setup->philos[i].meals_lock);
                if (setup->philos[i].meals < setup->times_to_eat)
                {
                    all_eaten = 0;
                    pthread_mutex_unlock(&setup->philos[i].meals_lock);
                    break;
                }
                pthread_mutex_unlock(&setup->philos[i].meals_lock);
            }

            if (all_eaten)
            {
                pthread_mutex_lock(&setup->stop_lock);
                setup->all_eaten = 1;
                setup->stop = 1;
                pthread_mutex_unlock(&setup->stop_lock);
                return (NULL);
            }
        }
        usleep(1000);
    }
    return (NULL);
}

int task(t_setup *setup)
{
    pthread_t monitor_thread;
    int i;

    if (pthread_create(&monitor_thread, NULL, check_starvation, setup) != 0)
    {
        printf("Error: Failed to create monitoring thread\n");
        return (1);
    }

    // Create all philosopher threads first
    i = 0;
    while (i < setup->philosophers)
    {
        if (pthread_create(&setup->philos[i].thread, NULL, philosopher_routine,
                &setup->philos[i]) != 0)
        {
            pthread_mutex_lock(&setup->stop_lock);
            setup->stop = 1;
            pthread_mutex_unlock(&setup->stop_lock);
            break;
        }
        i++;
    }

    // Join all created threads, even if stop is set
    i = 0;
    while (i < setup->philosophers)
    {
        if (pthread_join(setup->philos[i].thread, NULL) != 0)
        {
            printf("Error joining thread %d\n", i);
        }
        i++;
    }

    // Join monitor thread last
    pthread_join(monitor_thread, NULL);
    return (0);
}


int	main(int argc, char **argv)
{
	t_setup	setup;
	int		i;

	if (init_checks(argc, argv))
		return (0);
	if (init_values(argc, argv, &setup))
		return (0);
	init_philos(&setup);
	gettimeofday(&setup.start_time, NULL);

	// Run the simulation
	if (task(&setup) == 1)
		return (0);

	// Update elapsed_time before printing results
	pthread_mutex_lock(&setup.time_lock);
	setup.elapsed_time = get_timestamp(setup.start_time);
	pthread_mutex_unlock(&setup.time_lock);

	// Print final messages based on simulation outcome
	if (setup.all_eaten == 1)
	{
		printf("%lld All philosophers have eaten %d times\n",
			setup.elapsed_time, setup.times_to_eat);
	}
	//if (setup.is_dead != 0)
	//{
	//	printf("%lld %d died\n", setup.elapsed_time, setup.is_dead);
	//}

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

	// Cleanup resources
	for (i = 0; i < setup.philosophers; i++)
	{
		pthread_mutex_destroy(&setup.forks[i]);
		pthread_mutex_destroy(&setup.philos[i].meals_lock);
		pthread_mutex_destroy(&setup.philos[i].last_meal_lock);
		//pthread_mutex_destroy(setup.philos[i].left_fork);
		//pthread_mutex_destroy(setup.philos[i].right_fork);
	}
	pthread_mutex_destroy(&setup.write_lock);
	pthread_mutex_destroy(&setup.stop_lock);
	pthread_mutex_destroy(&setup.time_lock);
	free(setup.forks);
	free(setup.philos);
	return (0);
}
