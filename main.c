/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:11:19 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/01 13:02:38 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_setup	setup;

	if (argc < 5 || argc > 6)
	{
		printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat");
		printf(" time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		return (STOP_SIMULATION);
	}
	if (init_args(argc, argv, &setup))
		return (CONTINUE_SIMULATION);
	if (setup.nr_philosophers <= 0 || setup.nr_philosophers > 200)
	{
		printf("Error. Number of philosophers must be between 1 and 200.\n");
		return (STOP_SIMULATION);
	}
	init_values(&setup);
	init_philos(&setup);
	if (run_simulation(&setup))
	{
		cleanup_resources(&setup);
		return (STOP_SIMULATION);
	}
	cleanup_resources(&setup);
	return (0);
}
