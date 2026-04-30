#include "codexion.h"

void coder_init(t_rules *r)
{
    int i;

    i = 0;
    while (i < r->number_of_coders)
    {
        r->coders[i].id = i + 1;
        r->coders[i].left_dongle = i;
        r->coders[i].right_dongle = (i + 1) % r->number_of_coders;
        r->coders[i].number_of_compiles = 0;
        r->coders[i].stop = 0;
        r->coders[i].rules = r;
        i++;
    }
}

void rules_init(t_rules *r, char **av)
{
    r->number_of_coders = atoi(av[1]);
    r->time_to_burnout = atoi(av[2]);
    r->time_to_compile = atoi(av[3]);
    r->time_to_debug = atoi(av[4]);
    r->time_to_refactor = atoi(av[5]);
    r->number_of_compiles_required = atoi(av[6]);
    r->dongle_cooldown = atoi(av[7]);
    r->scheduler = av[8];
    r->coders = malloc(sizeof(t_coder) * r->number_of_coders);
    r->dongle_mutex = malloc(sizeof(pthread_mutex_t) * r->number_of_coders);
    r->stop = 0;
    if (!r->coders || !r->dongle_mutex)
    {
        printf("Error: malloc");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < r->number_of_coders; i++)
    {
        pthread_mutex_init(&r->dongle_mutex[i], NULL);
    }
    pthread_mutex_init(&r->data_mutex, NULL);
    pthread_mutex_init(&r->print_mutex, NULL);
}