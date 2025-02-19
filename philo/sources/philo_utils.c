/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: songevor <songevor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:07:33 by songevor          #+#    #+#             */
/*   Updated: 2025/02/19 16:07:33 by songevor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	parse_number(const char *str)
{
	int	result;
	int	sign;
	int	i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			sign = -1;
		else
			sign = 1;
	}
	while (str[i] >= '0' && str[i] <= '9')
		result = result * 10 + (str[i++] - '0');
	return (result * sign);
}

int	validate_string(const char *str)
{
	if (!str || !*str)
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
		if (!(*str >= '0' && *str++ <= '9'))
			return (0);
	return (1);
}

int	validate_parameters(t_tableStatus *table, int count, char **params)
{
	if (count != 5 && count != 6)
		return (0);
	if (!validate_string(params[1]) || !validate_string(params[2])
		|| !validate_string(params[3]) || !validate_string(params[4])
		|| (params[5] && !validate_string(params[5])))
		return (0);
	table->guest_count = parse_number(params[1]);
	table->survival_time = parse_number(params[2]);
	table->eating_duration = parse_number(params[3]);
	table->rest_duration = parse_number(params[4]);
	if (count == 6)
		table->meal_quota = parse_number(params[5]);
	else
		table->meal_quota = -1;
	return (table->guest_count > 0 && table->survival_time > 0
		&& table->eating_duration > 0 && table->rest_duration > 0
		&& (count != 6 || table->meal_quota > 0));
}

int	initialize_table(t_tableStatus *table, int count, char **params)
{
	if (!validate_parameters(table, count, params))
		return (0);
	table->dinner_active = 1;
	table->finished_guests = 0;
	if (!setup_utensils(table))
		return (0);
	if (!setup_table_locks(table))
	{
		free(table->utensils);
		return (0);
	}
	return (1);
}
