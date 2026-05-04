#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_coder t_coder;
typedef struct data_holder dh;

typedef struct s_heap {
    t_coder *waitlist[2];
    int count;
} t_heap;

typedef struct s_dongle {
    int id;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int is_active;
    long long ready_at;
    t_heap queue;
} t_dongle;

struct s_coder {
    int id;
    int compile_count;
    long long last_compile_start;
    t_dongle *left;
    t_dongle *right;
    int is_burned_out;
    long long request_time;
    pthread_t thread;
    pthread_mutex_t mutex;
    dh *dh;
};

struct data_holder {
    int number_of_coder;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refractor;
    int number_of_compiles_required;
    int dongle_cooldown;
    char* scheduler;
    int is_running;
    long long start_time;
    pthread_mutex_t mon_mutex;
    pthread_mutex_t print_mutex;
    pthread_cond_t stop_cond;
    t_coder *coders;
    t_dongle *dongles;  
};

//Utils
int is_valid_num(char *str);
int validation(int argc, char* argv[]);
void fill_fields(int **fields, int count, char **argv);
long long get_current_time_in_ms(void);
void print_log(dh *data, int coder_id, char *action);

// Init
int init_dongles(dh *dath);
int init_coders(dh *dath);

// Heap & Dongles
void swap_coders(t_coder **a, t_coder **b);
int is_higher_priority(t_coder *c1, t_coder *c2);
void insert_heap(t_heap *heap, t_coder *coder);
void remove_from_heap(t_heap *heap);
void request_dongle(t_coder *coder, t_dongle *dongle);

//Routine
void precise_sleep(long long time_to_sleeps_ms, dh *data);
void *coder_routine(void *arg);

#endif