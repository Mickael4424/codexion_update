#include "codexion.h"

void cleaning(t_rules *r)
{
    // destroy mutex
    pthread_mutex_destroy(&r->data_mutex);
    pthread_mutex_destroy(&r->print_mutex);
    
    for (int i = 0; i < r->number_of_coders; i++)
        pthread_mutex_destroy(&r->dongle_mutex[i]);
    
    // free malloc dongles et coders
    free(r->dongle_mutex);
    free(r->coders);
}