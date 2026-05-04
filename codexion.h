#ifndef CODEXION_H
#define CODEXION_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <ctype.h>

typedef struct s_rules t_rules;

typedef struct s_coder
{
    int id;
    int left_dongle;
    int right_dongle;
    long last_compiled;
    int number_of_compiles;
    
    pthread_t coder_thread;
    
    t_rules *rules;
} t_coder;

typedef struct s_request
{
    int coder_id;
    long request_time;
    long deadline;
    int granted;
    pthread_cond_t cond;
} t_request;

typedef struct s_dongle
{
    pthread_mutex_t mutex;
    t_request **queue;
    int size;
    int capacity;
    long last_release;
} t_dongle;

typedef struct s_rules
{
    int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    char * scheduler;
    
    long time_start;
    int stop;

    pthread_mutex_t data_mutex;
    pthread_mutex_t print_mutex;

    t_coder *coders;
    t_dongle *dongles;
} t_rules;

// dongle
int select_index(t_dongle *d, char *scheduler);
void request_dongle(t_dongle *d, t_coder *c);
void release_dongle(t_dongle *d, t_rules *r);

// init
void coder_init(t_rules *r);
void rules_init(t_rules *r, char **av);

// monitor
void *monitor(void *arg);

// parsing
int parsing(int ac, char **av);

// processing
void  take_dongles(t_coder *coder);
void *processing(void *arg);

// utils
long get_time(void);
void print_status(t_coder *coder, char *msg);
void cleaning(t_rules *r);

#endif
