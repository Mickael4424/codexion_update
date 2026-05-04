#include "codexion.h"


void *processing(void *arg)
{
    t_coder *c = (t_coder *)arg;
    t_rules *r = c->rules;

    while(1)
    {   
        pthread_mutex_lock(&r->data_mutex);
        if (r->stop)
        {
            pthread_mutex_unlock(&r->data_mutex);
            return NULL;
        }
        pthread_mutex_unlock(&r->data_mutex);

        // take dongles
        int first = c->left_dongle < c->right_dongle ? c->left_dongle : c->right_dongle;
        int second = c->left_dongle > c->right_dongle ? c->left_dongle : c->right_dongle;

        request_dongle(&r->dongles[first], c);
        print_status(c, "has taken a dongle");

        request_dongle(&r->dongles[second], c);
        print_status(c, "has taken a dongle");
        
        // coder is compiling
        pthread_mutex_lock(&r->data_mutex);
        c->last_compiled = get_time();
        c->number_of_compiles += 1;
        pthread_mutex_unlock(&r->data_mutex);
        
        print_status(c, "is compiling");
        usleep(r->time_to_compile * 1000);
        // printf("coder %d / number of compiles : %d\n", coder->id, coder->number_of_compiles);

        release_dongle(&r->dongles[first], r);
        release_dongle(&r->dongles[second], r);

        // coder is debugging
        print_status(c, "is debugging");
        usleep(r->time_to_debug * 1000);

        // coder is refactoring
        print_status(c, "is refactoring");
        usleep(r->time_to_refactor * 1000);
    }
    return NULL;
}
