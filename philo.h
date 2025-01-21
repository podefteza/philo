/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 11:24:35 by carlos-j          #+#    #+#             */
/*   Updated: 2025/01/21 01:43:53 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define MAX_PHILOSOPHERS 200

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

struct	s_setup;

typedef struct s_philos
{
	struct s_setup	*setup;

	int				id;
	int				meals;
	long long		last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meals_lock;
	pthread_mutex_t	last_meal_lock;
}					t_philos;

typedef struct s_setup
{
	t_philos		*philos;
	int				philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times_to_eat;
	struct timeval	start_time;
	long long		elapsed_time;
	pthread_mutex_t	time_lock;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	stop_lock;
	int				stop;
	int				all_eaten;
	int				is_dead;
}					t_setup;

int					ft_isdigit(int c);
long long			ft_atoi(const char *nptr);
long long			get_timestamp(struct timeval start_time);
int					get_stop_flag(t_setup *setup);
void				*check_starvation(void *arg);
int					log_action(t_philos *philo, const char *action);

// init.c
int					init_checks(int argc, char **argv);
int					init_args(int argc, char **argv, t_setup *setup);
int					init_values(t_setup *setup);
void				init_philos(t_setup *setup);

// take_forks.c
int					take_forks(t_philos *philo);
void				release_forks(t_philos *philo);

#endif
