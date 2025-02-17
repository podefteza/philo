/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 11:24:35 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/01 13:49:35 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define STOP_SIMULATION 1
# define CONTINUE_SIMULATION 0

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

struct	s_philo;
struct	s_setup;

typedef struct s_philo
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
}					t_philo;

typedef struct s_setup
{
	t_philo			*philo;
	int				nr_philosophers;
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
	int				all_eaten;
}					t_setup;

// utils.c
long long			ft_atoi(const char *nptr, int *error_flag);
long long			get_timestamp(struct timeval start_time);
void				set_stop_flag(t_setup *setup, int value);
int					get_stop_flag(t_setup *setup);

// init.c
int					init_args(int argc, char **argv, t_setup *setup);
void				init_values(t_setup *setup);
void				init_philos(t_setup *setup);

// take_forks.c
int					take_forks(t_philo *philo);
void				release_forks(t_philo *philo);

// philo_actions.c
int					philo_eating(t_philo *philo);
int					philo_sleeping(t_philo *philo);
int					philo_thinking(t_philo *philo);

// philo_checker.c
int					check_philosopher_status(t_setup *setup, int i,
						long long *last_meal, int *total_meals);
int					check_all_philosophers(t_setup *setup, int *total_meals);
void				*check_starvation(void *arg);

// philo_routine.c
int					run_simulation(t_setup *setup);

// cleanup_resources.c
void				cleanup_resources(t_setup *setup);

#endif
