/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dining_table.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: songevor <songevor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:58:14 by songevor          #+#    #+#             */
/*   Updated: 2025/02/19 15:58:14 by songevor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	print_status(t_tableStatus *table, int guest_id, const char *action)
{
	pthread_mutex_lock(&table->message_lock);
	pthread_mutex_lock(&table->table_lock);
	if (table->dinner_active)
		printf("%lld %d %s\n",
			current_timestamp() - table->start_time,
			guest_id + 1, action);
	pthread_mutex_unlock(&table->table_lock);
	pthread_mutex_unlock(&table->message_lock);
}

void	safe_wait(int duration, t_tableStatus *table)
{
	long long	name_time;

	name_time = current_timestamp() + duration;
	while (current_timestamp() < name_time)
	{
		pthread_mutex_lock(&table->table_lock);
		if (!table->dinner_active)
		{
			pthread_mutex_unlock(&table->table_lock);
			return ;
		}
		pthread_mutex_unlock(&table->table_lock);
		usleep(300);
	}
}

void	take_utensil(t_tableStatus *table, int utensil_id, int guest_id)
{
	pthread_mutex_lock(&table->utensils[utensil_id]);
	print_status(table, guest_id, "has taken a fork");
}

int	dine_alone(t_guest *guest, int utensil)
{
	take_utensil(guest->table, utensil, guest->position);
	safe_wait(guest->table->survival_time, guest->table);
	pthread_mutex_unlock(&guest->table->utensils[utensil]);
	return (0);
}

int	get_utensils(t_guest *guest)
{
	t_tableStatus	*table;
	int				first;
	int				second;

	table = guest->table;
	first = guest->position;
	second = (guest->position + 1) % table->guest_count;
	if (!is_dinner_active(table))
		return (0);
	if (table->guest_count == 1)
		return (dine_alone(guest, first));
	if (first < second)
	{
		take_utensil(table, first, guest->position);
		take_utensil(table, second, guest->position);
	}
	else
	{
		take_utensil(table, second, guest->position);
		take_utensil(table, first, guest->position);
	}
	return (1);
}
