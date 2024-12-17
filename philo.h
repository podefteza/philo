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
	struct timeval	start_time;
	long long		elapsed_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	stop_lock;
	int				stop;
}					t_setup;

int					ft_isdigit(int c);
long long			ft_atoi(const char *nptr);
long long			get_timestamp(struct timeval start_time);;
int					init_checks(int argc, char **argv);
int					init_values(int argc, char **argv, t_setup *setup);
void				init_philos(t_setup *setup);

#endif
