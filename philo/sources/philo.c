/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: songevor <songevor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:15:45 by songevor          #+#    #+#             */
/*   Updated: 2025/02/19 17:52:58 by songevor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	is_dinner_active(t_tableStatus *table)
{
	int	active;

	pthread_mutex_lock(&table->table_lock);
	active = table->dinner_active;
	pthread_mutex_unlock(&table->table_lock);
	return (active);
}

int	check_completion(t_tableStatus *table, t_guest *guests)
{
	int	i;
	int	all_finished;

	i = 0;
	all_finished = 1;
	while (i < table->guest_count)
	{
		pthread_mutex_lock(&guests[i].status_lock);
		if (guests[i].meals_taken < table->meal_quota)
			all_finished = 0;
		pthread_mutex_unlock(&guests[i].status_lock);
		i++;
	}
	return (all_finished);
}

void	monitor_dinner(t_tableStatus *table, t_guest *guests)
{
	int	i;
	int	found_death;

	while (1)
	{
		i = -1;
		found_death = 0;
		while (++i < table->guest_count)
		{
			pthread_mutex_lock(&guests[i].status_lock);
			if (check_guest_status(&guests[i]))
			{
				handle_guest_death(&guests[i]);
				found_death = 1;
				pthread_mutex_unlock(&guests[i].status_lock);
				break ;
			}
			pthread_mutex_unlock(&guests[i].status_lock);
		}
		if (kanach_kov(&table, &guests, found_death))
			break ;
		usleep(1000);
	}
}

int	kanach_kov(t_tableStatus **table, t_guest **guests, int found_death)
{
	if (found_death || ((*table)->meal_quota != -1
			&& check_completion(*table, *guests)))
	{
		pthread_mutex_lock(&(*table)->table_lock);
		(*table)->dinner_active = 0;
		pthread_mutex_unlock(&(*table)->table_lock);
		return (1);
	}
	return (0);
}
// if (found_death || (table->meal_quota != -1
		// 		&& check_completion(table, guests)))
		// {
		// 	pthread_mutex_lock(&table->table_lock);
		// 	table->dinner_active = 0;
		// 	pthread_mutex_unlock(&table->table_lock);
		// 	break ;
		// }