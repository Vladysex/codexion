/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlprysia <vlprysia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:49:30 by vlprysia          #+#    #+#             */
/*   Updated: 2026/05/05 00:49:32 by vlprysia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int init_dongles(dh *dath)
{
    int i = 0; 
    pthread_mutex_init(&dath->mon_mutex, NULL);
    pthread_mutex_init(&dath->print_mutex, NULL);
    pthread_cond_init(&dath->stop_cond, NULL); // Виправив опечатку ini -> init

    dath->dongles = malloc(sizeof(t_dongle) * dath->number_of_coder);
    if (!dath->dongles) return 0;

    while(i < dath->number_of_coder)
    {
        dath->dongles[i].id = i;
        dath->dongles[i].is_active = 0;
        dath->dongles[i].ready_at = 0;
        dath->dongles[i].queue.count = 0; // Важливо ініціалізувати чергу
        pthread_cond_init(&dath->dongles[i].cond, NULL);
        pthread_mutex_init(&dath->dongles[i].mutex, NULL);
        i++;
    }
    return 1;
}

int init_coders(dh *dath)
{
    int i = 0;
    dath->coders = malloc(sizeof(t_coder) * dath->number_of_coder);
    if (!dath->coders) return 0;

    while (i < dath->number_of_coder)
    {
        dath->coders[i].id = i+1;
        dath->coders[i].compile_count = 0;
        dath->coders[i].is_burned_out = 0;
        dath->coders[i].last_compile_start = 0;
        pthread_mutex_init(&dath->coders[i].mutex, NULL);
        dath->coders[i].left = &dath->dongles[i];
        dath->coders[i].right = &dath->dongles[(i+1) % dath->number_of_coder];
        dath->coders[i].dh = dath;
        i++;
    }
    return 1;
}