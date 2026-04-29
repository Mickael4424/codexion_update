#include "codexion.h"
#include <stdlib.h>


void *processing(void *arg)
{
    t_coder *coder = (t_coder *)arg;

    while(1)
    {   
        pthread_mutex_lock(&coder->rules->data_mutex);
        if (coder->rules->stop)
        {
            printf("Stop");
            pthread_mutex_unlock(&coder->rules->data_mutex);
            return NULL;
        }
        pthread_mutex_unlock(&coder->rules->data_mutex);

        int first = min(coder->left_dongle, coder->right_dongle);
        int second = max(coder->left_dongle, coder->right_dongle);

        pthread_mutex_lock(&coder->rules->dongle_mutex[first]);
        pthread_mutex_lock(&coder->rules->dongle_mutex[second]);

        pthread_mutex_lock(&coder->rules->print_mutex);
        printf("%d has taken a dongle\n", coder->id);
        printf("%d has taken a dongle\n", coder->id);
        pthread_mutex_unlock(&coder->rules->print_mutex);
        
        // coder is compiling
        pthread_mutex_lock(&coder->rules->data_mutex);
        coder->number_of_compiles += 1;
        printf("%d is compiling\n", coder->id);
        printf("coder %d / number of compiles : %d\n", coder->id, coder->number_of_compiles);
        pthread_mutex_unlock(&coder->rules->data_mutex);
        usleep(coder->rules->time_to_compile * 1000);

        // coder is debugging
        pthread_mutex_lock(&coder->rules->print_mutex);
        printf("%d is debugging\n", coder->id);
        pthread_mutex_unlock(&coder->rules->print_mutex);
        usleep(coder->rules->time_to_debug * 1000);

        pthread_mutex_unlock(&coder->rules->dongle_mutex[second]);
        pthread_mutex_unlock(&coder->rules->dongle_mutex[first]);

        // coder is refactoring
        pthread_mutex_lock(&coder->rules->print_mutex);
        printf("%d is refactoring\n", coder->id);
        pthread_mutex_unlock(&coder->rules->print_mutex);
        usleep(coder->rules->time_to_refactor * 1000);
    }
    return NULL;
}

void *monitor(void *arg)
{
    t_rules *r = (t_rules *)arg;
    while (1)
    {
        for (int i = 0; i < r->number_of_coders; i++)
        {
            pthread_mutex_lock(&r->data_mutex);
            if (r->coders[i].number_of_compiles >= 5)
            {
                r->stop = 1;
                pthread_mutex_unlock(&r->data_mutex);
                return NULL;
            }
            pthread_mutex_unlock(&r->data_mutex);
        }
        usleep(100000);
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
    // 3. LANCEMENT DU PROCESSUS
    // 4. PROCESSUS ARRET
    return (0);
}