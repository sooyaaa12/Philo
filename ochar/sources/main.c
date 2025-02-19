#include "../includes/philo.h"

int main(int argc, char **argv)
{
    t_tableStatus table;
    t_guest *guests;
    pthread_t *guest_threads;
    int i;

    table.start_time = current_timestamp();
    if (!initialize_table(&table, argc, argv))
    {
        printf("Error: Invalid arguments or memory allocation failed.\n");
        return (1);
    }

    if (!allocate_resources(&table, &guests, &guest_threads))
    {
        printf("Error: Memory allocation failed.\n");
        return (1);
    }
    
    setup_guests(&table, guests);

    for (i = 0; i < table.guest_count; i++)
    {
        if (pthread_create(&guest_threads[i], NULL, guest_routine, &guests[i]) != 0)
        {
            printf("Error: Failed to create thread.\n");
            cleanup_dinner(&table, guests, guest_threads);
            return (1);
        }
    }
    monitor_dinner(&table, guests);
    cleanup_dinner(&table, guests, guest_threads);
    return (0);
}
