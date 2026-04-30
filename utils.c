#include "codexion.h"

long get_time(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void print_status(t_coder *coder, char *msg)
{
    pthread_mutex_lock(&coder->rules->data_mutex);
    if (!coder->rules->stop)
    {
        pthread_mutex_lock(&coder->rules->print_mutex);
        printf("%ld %d %s\n", get_time() - coder->rules->time_start, coder->id, msg);
        pthread_mutex_unlock(&coder->rules->print_mutex);
    }
    pthread_mutex_unlock(&coder->rules->data_mutex);
}
