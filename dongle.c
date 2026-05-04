#include "codexion.h"


int select_index(t_dongle *d, char *scheduler)
{
    int best = 0;

    for (int i = 1; i < d->size; i++)
    {
        if (!strcmp(scheduler, "fifo"))
        {
            if (d->queue[i]->request_time < d->queue[best]->request_time)
                best = i;
        }
        else
        {
            if (d->queue[i]->deadline < d->queue[best]->deadline)
                best = i;
        }
    }
    return best;
}

void request_dongle(t_dongle *d, t_coder *c)
{
    t_request req;

    pthread_mutex_lock(&d->mutex);

    pthread_mutex_lock(&c->rules->data_mutex);
    long last = c->last_compiled;
    pthread_mutex_unlock(&c->rules->data_mutex);

    req.coder_id = c->id;
    req.request_time = get_time();
    req.deadline = last + c->rules->time_to_burnout;
    req.granted = 0;

    pthread_cond_init(&req.cond, NULL);

    d->queue[d->size++] = &req;

    while (!req.granted ||
          (get_time() - d->last_release < c->rules->dongle_cooldown))
    {
        pthread_cond_wait(&req.cond, &d->mutex);
    }

    pthread_mutex_unlock(&d->mutex);
    pthread_cond_destroy(&req.cond);
}

void release_dongle(t_dongle *d, t_rules *r)
{
    pthread_mutex_lock(&d->mutex);

    d->last_release = get_time();

    if (d->size > 0)
    {
        int idx = select_index(d, r->scheduler);
        t_request *next = d->queue[idx];

        next->granted = 1;
        pthread_cond_signal(&next->cond);

        for (int i = idx; i < d->size - 1; i++)
            d->queue[i] = d->queue[i + 1];

        d->size--;
    }

    pthread_mutex_unlock(&d->mutex);
}