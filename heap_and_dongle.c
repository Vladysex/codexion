/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_and_dongle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlprysia <vlprysia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:49:56 by vlprysia          #+#    #+#             */
/*   Updated: 2026/05/05 00:49:58 by vlprysia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void swap_coders(t_coder **a, t_coder **b)
{
    t_coder *temp = *a;
    *a = *b;
    *b = temp;
}

int is_higher_priority(t_coder *c1, t_coder *c2)
{
    if(strcmp(c1->dh->scheduler, "edf") == 0)
    {
        long long d1 = c1->last_compile_start + c1->dh->time_to_burnout;
        long long d2 = c2->last_compile_start + c2->dh->time_to_burnout; // Виправив c1->dh на c2->dh
        return (d1 < d2);
    }
    return (c1->request_time < c2->request_time);
}

void insert_heap(t_heap *heap, t_coder *coder)
{
    int current = heap->count;
    int parent;

    heap->waitlist[current] = coder; // Виправив назву масиву на waitlist
    heap->count++;

    while(current > 0)
    {
        parent = (current - 1) / 2;
        if(is_higher_priority(heap->waitlist[current], heap->waitlist[parent]))
        {
            swap_coders(&heap->waitlist[current], &heap->waitlist[parent]);
            current = parent;
        }
        else break;
    }
}

void remove_from_heap(t_heap *heap)
{
    if (heap->count == 0) return;
    heap->waitlist[0] = heap->waitlist[heap->count - 1];
    heap->count--;
}

void request_dongle(t_coder *coder, t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->mutex);
    coder->request_time = get_current_time_in_ms();
    insert_heap(&dongle->queue, coder);

    while(dongle->is_active==1 || 
        get_current_time_in_ms()<dongle->ready_at ||
        dongle->queue.waitlist[0]!= coder)
    {
        pthread_cond_wait(&dongle->cond, &dongle->mutex);
    }
    dongle->is_active = 1;
    remove_from_heap(&dongle->queue);
    pthread_mutex_unlock(&dongle->mutex);
    print_log(coder->dh, coder->id, "has taken a dongle");
}