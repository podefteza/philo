/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 11:16:53 by carlos-j          #+#    #+#             */
/*   Updated: 2025/01/28 18:46:48 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	ft_atoi(const char *nptr, int *error_flag)
{
	long long	number;
	int			signal;

	number = 0;
	signal = 1;
	*error_flag = 0;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == ' ')
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			signal = -1;
		nptr++;
	}
	while (*nptr)
	{
		if (!(*nptr >= '0' && *nptr <= '9'))
		{
			*error_flag = 1;
			return (0);
		}
		number = (number * 10) + (*nptr - '0');
		nptr++;
	}
	return (number * signal);
}

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
