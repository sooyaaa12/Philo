#include "../includes/philo.h"

long long current_timestamp(void)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return ((now.tv_sec * 1000LL) + (now.tv_usec / 1000));
}

void eat_meal(t_guest *guest)
{
    t_tableStatus *table;
    int first;
    int second;

    table = guest->table;
    first = guest->position;
    second = (guest->position + 1) % table->guest_count;

    pthread_mutex_lock(&guest->status_lock);
    guest->last_meal = current_timestamp();
    guest->meals_taken++;
    pthread_mutex_unlock(&guest->status_lock);

    print_status(table, guest->position, "is eating");
    safe_wait(table->eating_duration, table);
    pthread_mutex_unlock(&table->utensils[first]);
    pthread_mutex_unlock(&table->utensils[second]);
}

void *guest_routine(void *arg)
{
    t_guest *guest;
    t_tableStatus *table;

    guest = (t_guest *)arg;
    table = guest->table;

    if (guest->position % 2)
        usleep(1500);

    while (is_dinner_active(table))
    {
        if (!get_utensils(guest))
            break;
        eat_meal(guest);
        print_status(table, guest->position, "is resting");
        safe_wait(table->rest_duration, table);
        print_status(table, guest->position, "is waiting");
    }
    return (NULL);
}

int check_guest_status(t_guest *guest)
{
    long long current;
    long long last_meal;

    current = current_timestamp();
    last_meal = guest->last_meal;
    return (current - last_meal > guest->table->survival_time);
}


void handle_guest_death(t_guest *guest)
{
    t_tableStatus *table;

    table = guest->table;
    pthread_mutex_lock(&table->table_lock);
    if (table->dinner_active)
    {
        table->dinner_active = 0;
        pthread_mutex_unlock(&table->table_lock);
        pthread_mutex_lock(&table->message_lock);
        printf("%lld %d died\n",
            current_timestamp() - table->start_time,
            guest->position + 1);
        pthread_mutex_unlock(&table->message_lock);
    }
    else
        pthread_mutex_unlock(&table->table_lock);
}