#include "codexion.h"

void dongle_init(t_rules *r)
{
    int i;

    r->dongles = malloc(sizeof(t_dongle) * r->number_of_coders); 
    if (!r->dongles)
    {
        printf("Error: malloc dongle");
        exit(EXIT_FAILURE);
    }
    
    i = 0;
    while (i < r->number_of_coders)
    {
        pthread_mutex_init(&r->dongles[i].mutex, NULL);
        
        r->dongles[i].queue = malloc(sizeof(t_request *) * r->number_of_coders);
        if (!r->dongles[i].queue)
        {
            printf("Error: queue dongle");
            exit(EXIT_FAILURE);
        }
        r->dongles[i].size = 0;
        r->dongles[i].capacity = r->number_of_coders;
        r->dongles[i].last_release = 0;
        i++;
    }

}

void coder_init(t_rules *r)
{
    int i;

    r->coders = malloc(sizeof(t_coder) * r->number_of_coders);
    if (!r->coders)
    {
        printf("Error: malloc");
        exit(EXIT_FAILURE);
    }
    i = 0;
    while (i < r->number_of_coders)
    {
        r->coders[i].id = i + 1;
        r->coders[i].left_dongle = i;
        r->coders[i].right_dongle = (i + 1) % r->number_of_coders;
        r->coders[i].last_compiled = 0;
        r->coders[i].number_of_compiles = 0;
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

    r->stop = 0;

    pthread_mutex_init(&r->data_mutex, NULL);
    pthread_mutex_init(&r->print_mutex, NULL);

    dongle_init(r);
}