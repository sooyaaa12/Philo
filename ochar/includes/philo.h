#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_tableStatus
{
    int             guest_count;
    int             survival_time;
    int             eating_duration;
    int             rest_duration;
    int             meal_quota;
    int             dinner_active;
    int             finished_guests;
    long long       start_time;
    pthread_mutex_t *utensils;
    pthread_mutex_t table_lock;
    pthread_mutex_t message_lock;
}   t_tableStatus;

typedef struct s_guest
{
    int             position;
    int             meals_taken;
    long long       last_meal;
    pthread_mutex_t status_lock;
    t_tableStatus   *table;
}   t_guest;

// Initialization functions
int         initialize_table(t_tableStatus *table, int count, char **params);
int         setup_utensils(t_tableStatus *table);
int         setup_table_locks(t_tableStatus *table);
int         allocate_resources(t_tableStatus *table, t_guest **guests, 
                pthread_t **guest_threads);
void        setup_guests(t_tableStatus *table, t_guest *guests);

// Utility functions
int         parse_number(const char *str);
int         validate_string(const char *str);
int         validate_parameters(t_tableStatus *table, int count, char **params);
long long   current_timestamp(void);
void        safe_wait(int duration, t_tableStatus *table);

// Dining operations
void        take_utensil(t_tableStatus *table, int utensil_id, int guest_id);
int         get_utensils(t_guest *guest);
void        eat_meal(t_guest *guest);
int         dine_alone(t_guest *guest, int utensil);
void        *guest_routine(void *arg);

// Status and monitoring
void        print_status(t_tableStatus *table, int guest_id, const char *action);
int         check_guest_status(t_guest *guest);
void        handle_guest_death(t_guest *guest);
int         is_dinner_active(t_tableStatus *table);
int         check_completion(t_tableStatus *table, t_guest *guests);
void        monitor_dinner(t_tableStatus *table, t_guest *guests);

// Cleanup
void        cleanup_dinner(t_tableStatus *table, t_guest *guests, 
                pthread_t *threads);

#endif