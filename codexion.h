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
    int number_of_compiles;
    
    pthread_t coder_thread;
    
    t_rules *rules;
} t_coder;

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
    int stop;

    pthread_mutex_t *dongle_mutex;
    pthread_mutex_t data_mutex;
    pthread_mutex_t print_mutex;

    t_coder *coders;
} t_rules;

// parsing
int check_nb_arguments (int ac);
int check_positiv_int(char **av);
int parsing(int ac, char **av);
int check_last_arg(char **av);

// init
void coder_init(t_rules *r);
void rules_init(t_rules *r, char **av);

// cleaning
void cleaning(t_rules *r);

#endif
