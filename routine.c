/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlprysia <vlprysia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:50:15 by vlprysia          #+#    #+#             */
/*   Updated: 2026/05/05 00:50:27 by vlprysia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void precise_sleep(long long time_to_sleeps_ms, dh *data)
{
    struct timeval  tv;
    struct timespec ts;
    long long total_usec;

    gettimeofday(&tv, NULL);
    total_usec = tv.tv_usec + (time_to_sleeps_ms * 1000);
    ts.tv_sec = tv.tv_sec + (total_usec / 1000000);
    ts.tv_nsec = (total_usec % 1000000) * 1000;

    pthread_mutex_lock(&data->mon_mutex);
    if (data->is_running == 1)
    {
        pthread_cond_timedwait(&data->stop_cond, &data->mon_mutex, &ts);
    }
    pthread_mutex_unlock(&data->mon_mutex);
}

void *coder_routine(void *arg)
{
    t_coder *coder = (t_coder *)arg;

    while (1)
    {
        pthread_mutex_lock(&coder->dh->mon_mutex);
        if (coder->dh->is_running == 0 || coder->compile_count == coder->dh->number_of_compiles_required)
        {
            pthread_mutex_unlock(&coder->dh->mon_mutex);
            break;
        }
        pthread_mutex_unlock(&coder->dh->mon_mutex);

        print_log(coder->dh, coder->id, "is thinking about compiling...");
        precise_sleep(coder->dh->time_to_compile, coder->dh);
        
        coder->compile_count++;
    }
    return NULL;
}