/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlprysia <vlprysia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:50:44 by vlprysia          #+#    #+#             */
/*   Updated: 2026/05/05 00:51:04 by vlprysia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int main(int argc, char* argv[])
{
    struct data_holder *dh = malloc(sizeof(struct data_holder));
    int i = 0;

    if (!dh) return (1);

    int *fields[] = {
        &dh->number_of_coder, &dh->time_to_burnout, &dh->time_to_compile,
        &dh->time_to_debug, &dh->time_to_refractor, &dh->number_of_compiles_required,
        &dh->dongle_cooldown
    };

    if(validation(argc, argv))
    {
        fill_fields(fields, 7, argv);
        dh->scheduler = argv[8];
    }
    else return (free(dh), 1);

    if (!init_dongles(dh) || !init_coders(dh)) return (1);
    
    dh->is_running = 1;
    dh->start_time = get_current_time_in_ms();

    while (i < dh->number_of_coder)
    {
        dh->coders[i].last_compile_start = dh->start_time; 
        pthread_create(&dh->coders[i].thread, NULL, coder_routine, &dh->coders[i]);
        i++;
    }

    usleep(2000000); 

    pthread_mutex_lock(&dh->mon_mutex);
    dh->is_running = 0;
    pthread_cond_broadcast(&dh->stop_cond); 
    pthread_mutex_unlock(&dh->mon_mutex);

    i = 0;
    while (i < dh->number_of_coder)
    {
        pthread_join(dh->coders[i].thread, NULL);
        i++;
    }

    free(dh->coders);
    free(dh->dongles);
    free(dh);

    printf("Simulation test finished!\n");
    return 0;
}