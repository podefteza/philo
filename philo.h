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
#define PHILO_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct setup
{
	int philosophers;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int times_to_eat;

}	t_setup;


int	ft_isdigit(int c);
long long	ft_atoi(const char *nptr);


#endif
