#include "codexion.h"


void *monitor(void *arg)
{
    t_rules *r = (t_rules *)arg;
    
    while (1)
    {
        for (int i = 0; i < r->number_of_coders; i++)
        {
            pthread_mutex_lock(&r->data_mutex);

            long now = get_time();

            if ((now - r->coders[i].last_compiled) > r->time_to_burnout)
            {
                printf("%ld %d burned out\n",
                    now - r->time_start, r->coders[i].id);
                r->stop = 1;
                pthread_mutex_unlock(&r->data_mutex);
                return NULL;
            }
            pthread_mutex_unlock(&r->data_mutex);
        }
        usleep(1000);
    }
}