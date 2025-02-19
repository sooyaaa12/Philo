/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: songevor <songevor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:00:20 by songevor          #+#    #+#             */
/*   Updated: 2025/02/19 16:00:20 by songevor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	initialize_params(int argc, char **argv, t_tableStatus *table)
{
	table->start_time = current_timestamp();
	if (argc != 5 && argc != 6)
	{
		printf("Error: Invalid number of arguments.\n");
		return (0);
	}
	if (!validate_parameters(table, argc, argv))
	{
		printf("Error: Invalid arguments.\n");
		return (0);
	}
	if (!initialize_table(table, argc, argv))
	{
		printf("Error: Invalid arguments or memory allocation failed.\n");
		return (0);
	}
	return (1);
}

int	normhandle(t_tableStatus *table, t_guest *guests,
					pthread_t *guest_threads)
{
	cleanup_dinner(table, guests, guest_threads);
	return (1);
}

int	normhandle2(t_tableStatus *table, t_guest *guests,
					pthread_t *guest_threads)
{
	cleanup_dinner(table, guests, guest_threads);
	return (0);
}

int	main(int argc, char **argv)
{
	t_tableStatus	table;
	t_guest			*guests;
	pthread_t		*guest_threads;
	int				i;

	i = -1;
	if (!initialize_params(argc, argv, &table))
		return (1);
	if (!allocate_resources(&table, &guests, &guest_threads))
	{
		printf("Error: Memory allocation failed.\n");
		return (1);
	}
	setup_guests(&table, guests);
	while (++i < table.guest_count)
	{
		if (pthread_create(&guest_threads[i], NULL,
				guest_routine, &guests[i]) != 0)
		{
			printf("Error: Failed to create thread.\n");
			return (normhandle(&table, guests, guest_threads));
		}
	}
	monitor_dinner(&table, guests);
	return (normhandle2(&table, guests, guest_threads));
}
