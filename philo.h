/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 11:24:35 by carlos-j          #+#    #+#             */
/*   Updated: 2024/12/12 11:26:08 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

// keep tracking of the current time
// keep track of the number of times each philo ate + last time he ate

struct setup;

typedef struct philos
{
	struct setup	*setup;

	int				id;
	int				meals;
	long long		last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}					t_philos;

typedef struct setup
{
	t_philos		*philos;

	int				philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times_to_eat;
	long long		start_time;
	long long		elapsed_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	int				stop;
}					t_setup;

int					ft_isdigit(int c);
long long			ft_atoi(const char *nptr);

#endif
