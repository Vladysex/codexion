/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlprysia <vlprysia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:49:09 by vlprysia          #+#    #+#             */
/*   Updated: 2026/05/05 00:49:12 by vlprysia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int is_valid_num(char *str)
{
    int i = 0;
    if (!str || str[0] == '\0') return 0;
    if (str[i] == '+') i++;
    if (str[i] == '\0') return 0;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9') return 0;
        i++;
    }
    return 1;
}

int validation(int argc, char* argv[])
{
    int i = 1;
    if (argc != 9)
    {
        printf("ERROR! Has to be exactly 9 parameters!\n");
        return 0;
    }
    while(i < 8)
    {
        if (!is_valid_num(argv[i]))
        {
            printf("%s > - invalid format\n", argv[i]);
            return 0;
        }
        i++;
    }
    return 1;
}

void fill_fields(int **fields, int count, char **argv)
{
    int i = 0;
    while(i < count)
    {
        *fields[i] = atoi(argv[i+1]);
        i++;
    }
}

long long get_current_time_in_ms(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((long long)time.tv_sec * 1000) + (time.tv_usec / 1000);
}

void print_log(dh *data, int coder_id, char *action)
{
    long long time;
    pthread_mutex_lock(&data->print_mutex);
    if(data->is_running == 1)
    {
        time = get_current_time_in_ms() - data->start_time;
        printf("%lld %d %s\n", time, coder_id, action);
    }
    pthread_mutex_unlock(&data->print_mutex);
}