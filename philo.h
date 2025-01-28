/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 11:24:35 by carlos-j          #+#    #+#             */
/*   Updated: 2025/01/28 17:23:18 by carlos-j         ###   ########.fr       */
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

struct	s_philos;
struct	s_setup;

typedef struct s_philos
{
	struct s_setup	*setup; 		// pointer to the setup struct

	int				id; 			// id of the philo
	int				meals; 			// number of meals eaten
	long long		last_meal; 		// timestamp of the last meal
	pthread_t		thread; 		// thread of the philo
	pthread_mutex_t	*left_fork; 	// his own fork
	pthread_mutex_t	*right_fork; 	// the fork of the next philo
	pthread_mutex_t	meals_lock; 	// lock for the meals variable
	pthread_mutex_t	last_meal_lock;	// lock for the last_meal variable
}					t_philos;

typedef struct s_setup
{
	t_philos		*philos; 			// array of philos
	int				nr_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times_to_eat;
	struct timeval	start_time;			// start time of the simulation
	long long		elapsed_time;		// elapsed time since the start of the simulation
	pthread_mutex_t	*forks;				// array of forks
	pthread_mutex_t	write_lock;			// lock for the write function
	pthread_mutex_t	stop_lock;			// lock for the simulation stop variable
	int				stop;				// flag to stop the simulation
	int				all_eaten;			// flag to stop the simulation when all philosophers have eaten
	int				is_dead;			// flag to stop the simulation when a philosopher dies
}					t_setup;

// utils.c
long long			ft_atoi(const char *nptr, int *error_flag);
long long			get_timestamp(struct timeval start_time);
void				set_stop_flag(t_setup *setup, int value);
int					get_stop_flag(t_setup *setup);

// init.c
int					init_args(int argc, char **argv, t_setup *setup);
int					init_values(t_setup *setup);
void				init_philos(t_setup *setup);

// take_forks.c
int					take_forks(t_philos *philo);
void				release_forks(t_philos *philo);

// philo_actions.c
int					philo_eating(t_philos *philo);
int					philo_sleeping(t_philos *philo);
int					philo_thinking(t_philos *philo);

// philo_checker.c
int					check_philosopher_status(t_setup *setup, int i,
						long long *last_meal, int *total_meals);
int					check_all_philosophers(t_setup *setup, int *total_meals);
void				*check_starvation(void *arg);

// philo_routine.c
int					task(t_setup *setup);

// cleanup_resources.c
void				cleanup_resources(t_setup *setup);

#endif
