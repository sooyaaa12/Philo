#include "../includes/philo.h"

int setup_utensils(t_tableStatus *table)
{
    int index;

    index = 0;
    table->utensils = malloc(sizeof(pthread_mutex_t) * table->guest_count);
    if (!table->utensils)
        return (0);
    while (index < table->guest_count)
    {
        if (pthread_mutex_init(&table->utensils[index], NULL) != 0)
        {
            while (--index >= 0)
                pthread_mutex_destroy(&table->utensils[index]);
            free(table->utensils);
            return (0);
        }
        index++;
    }
    return (1);
}

int setup_table_locks(t_tableStatus *table)
{
    if (pthread_mutex_init(&table->table_lock, NULL) != 0)
        return (0);
    if (pthread_mutex_init(&table->message_lock, NULL) != 0)
    {
        pthread_mutex_destroy(&table->table_lock);
        return (0);
    }
    return (1);
}


int allocate_resources(t_tableStatus *table, t_guest **guests, pthread_t **guest_threads)
{
    *guests = malloc(sizeof(t_guest) * table->guest_count);
    *guest_threads = malloc(sizeof(pthread_t) * table->guest_count);
    if (!*guests || !*guest_threads)
    {
        free(*guests);
        free(*guest_threads);
        return (0);
    }
    return (1);
}

void setup_guests(t_tableStatus *table, t_guest *guests)
{
    int i;

    i = 0;
    while (i < table->guest_count)
    {
        guests[i].position = i;
        guests[i].meals_taken = 0;
        guests[i].last_meal = table->start_time;
        guests[i].table = table;
        pthread_mutex_init(&guests[i].status_lock, NULL);
        i++;
    }
}

void cleanup_dinner(t_tableStatus *table, t_guest *guests, pthread_t *threads)
{
    int i;

    i = 0;
    while (i < table->guest_count)
    {
        pthread_join(threads[i], NULL);
        pthread_mutex_destroy(&guests[i].status_lock);
        pthread_mutex_destroy(&table->utensils[i]);
        i++;
    }
    pthread_mutex_destroy(&table->table_lock);
    pthread_mutex_destroy(&table->message_lock);
    free(table->utensils);
    free(guests);
    free(threads);
}
