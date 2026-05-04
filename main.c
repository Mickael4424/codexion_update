#include "codexion.h"


int main(int ac, char **av)
{
    t_rules r;
    pthread_t monitor_thread;

    // parsing
    if (parsing(ac, av))
        return(1);
    
    // init 
    rules_init(&r, av);
    coder_init(&r);

    r.time_start = get_time();

    // thread creation 
    for (int i = 0; i < r.number_of_coders; i++)
    {
        pthread_create(&r.coders[i].coder_thread, NULL,
            processing, &r.coders[i]);
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