#include "codexion.h"

void  take_dongles(t_coder *coder)
{
    int first;
    int second;
    int left;
    int right;

    left = coder->left_dongle;
    right = coder->right_dongle;

    first = left < right ? left : right;
    second = left > right ? left : right;

    pthread_mutex_lock(&coder->rules->dongle_mutex[first]);
    print_status(coder, "has taken a dongle");
    pthread_mutex_lock(&coder->rules->dongle_mutex[second]);
    print_status(coder, "has taken a dongle");
}

void *processing(void *arg)
{
    t_coder *coder = (t_coder *)arg;

    while(1)
    {   
        pthread_mutex_lock(&coder->rules->data_mutex);
        if (coder->rules->stop == coder->rules->number_of_coders)
        {
            pthread_mutex_unlock(&coder->rules->data_mutex);
            return NULL;
        }
        pthread_mutex_unlock(&coder->rules->data_mutex);

        // take dongles
        take_dongles(coder);
        
        // coder is compiling
        pthread_mutex_lock(&coder->rules->data_mutex);
        coder->number_of_compiles += 1;
        pthread_mutex_unlock(&coder->rules->data_mutex);
        pthread_mutex_lock(&coder->rules->data_mutex);
        coder->last_compiled = get_time() - coder->rules->time_start;
        pthread_mutex_unlock(&coder->rules->data_mutex);
        print_status(coder, "is compiling");
        
        // printf("coder %d / number of compiles : %d\n", coder->id, coder->number_of_compiles);
        
        usleep(coder->rules->time_to_compile * 1000);

        // coder is debugging
        print_status(coder, "is debugging");
        usleep(coder->rules->time_to_debug * 1000);

        pthread_mutex_unlock(&coder->rules->dongle_mutex[coder->left_dongle]);
        pthread_mutex_unlock(&coder->rules->dongle_mutex[coder->right_dongle]);

        // coder is refactoring
        print_status(coder, "is refactoring");
        usleep(coder->rules->time_to_refactor * 1000);
    }
    return NULL;
}

void *monitor(void *arg)
{
    t_rules *r;
    
    r = (t_rules *)arg;
    
    while (1)
    {
        for (int i = 0; i < r->number_of_coders; i++)
        {
            if ((get_time() - r->time_start - r->coders[i].last_compiled) > r->time_to_burnout)
            {
                r->stop = r->number_of_coders;
                printf("%ld %d burned out\n",
                    get_time() - r->time_start, r->coders[i].id);
                return NULL;
            }
        }
        for (int i = 0; i < r->number_of_coders; i++)
        {
            pthread_mutex_lock(&r->data_mutex);
            if (r->coders[i].number_of_compiles >= 5)
            {
                r->coders[i].stop = 1;
                pthread_mutex_unlock(&r->data_mutex);
            }
            pthread_mutex_unlock(&r->data_mutex);
        }
        r->stop = 0;
        for (int i = 0; i < r->number_of_coders; i++)
        {
            pthread_mutex_lock(&r->data_mutex);
            if (r->coders[i].stop)
            {
                r->stop += 1;
                pthread_mutex_unlock(&r->data_mutex);
            }
            pthread_mutex_unlock(&r->data_mutex);
        }
        if (r->stop == r->number_of_coders)
        {
            printf("all coders compiled %d times\n", r->stop);
            for (int i = 0; i < r->number_of_coders; i++)
            {
                printf("coder %d compiled %d times\n",
                    r->coders[i].id, r->coders[i].number_of_compiles);
                // printf("coder %d : last compiled %ld\n", r->coders[i].id, r->coders[i].last_compiled);
            }
            return NULL;
        }
        usleep(10000);
    }
    
    return NULL;
}

int main(int ac, char **av)
{
    t_rules r;
    pthread_t monitor_thread;

    // parsing
    if (parsing(ac, av) == 1)
    {
        return(1);
    }
    
    // init 
    rules_init(&r, av);
    coder_init(&r);
    r.time_start = get_time();

    // thread creation 
    for (int i = 0; i < r.number_of_coders; i++)
    {
        pthread_create(&r.coders[i].coder_thread, NULL, processing, &r.coders[i]);
    }
    pthread_create(&monitor_thread, NULL, monitor, &r);

    // thread processing
    for (int i = 0; i < r.number_of_coders; i++)
    {
        pthread_join(r.coders[i].coder_thread, NULL);
    }
    pthread_join(monitor_thread, NULL);
    cleaning(&r);
    return (0);
}