#include "codexion.h"

long get_time(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void print_status(t_coder *coder, char *msg)
{
    pthread_mutex_lock(&coder->rules->print_mutex);
    pthread_mutex_lock(&coder->rules->data_mutex);
    if (!coder->rules->stop)
    {
        printf("%ld %d %s\n",
            get_time() - coder->rules->time_start,
            coder->id, msg);
    }
    pthread_mutex_unlock(&coder->rules->data_mutex);
    pthread_mutex_unlock(&coder->rules->print_mutex);
}

void cleaning(t_rules *r)
{
    for (int i = 0; i < r->number_of_coders; i++)
    {
        pthread_mutex_destroy(&r->dongles[i].mutex);
        free(r->dongles[i].queue);
    }
    // free malloc dongles et coders
    free(r->dongles);
    free(r->coders);

    // destroy mutex
    pthread_mutex_destroy(&r->data_mutex);
    pthread_mutex_destroy(&r->print_mutex);
}