/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlprysia <vlprysia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 17:39:09 by vlprysia          #+#    #+#             */
/*   Updated: 2026/04/25 16:53:29 by vlprysia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int is_valid_num(char *str);



typedef struct s_dongle{
	int id;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int is_active;
	long long ready_at;

} t_dongle;

typedef struct s_coder {
	int id;
	int compile_count;
	long long last_compile_start;
	t_dongle *left;
	t_dongle *right;
	int is_burned_out;
	pthread_t thread;
	pthread_mutex_t mutex;
	struct data_holder *dh;
	
} t_coder;

typedef struct data_holder {
	int number_of_coder;
	int time_to_burnout;
	int time_to_compile;
	int time_to_debug;
	int time_to_refractor;
	int number_of_compiles_required;
	int dongle_cooldown;
	char* scheduler;
	int is_running;
	pthread_mutex_t mon_mutex;
	pthread_mutex_t print_mutex;
	t_coder *coders;
	t_dongle *dongles;	
} dh;


int init_dongles(dh *dath){
	int i = 0; 
	
	pthread_mutex_init(&dath->mon_mutex, NULL);
	pthread_mutex_init(&dath->print_mutex, NULL);

	dath->dongles = malloc(sizeof(t_dongle) * dath->number_of_coder);
	if (!dath->dongles)
		return (free(dath->dongles), 0);

	while(i < dath->number_of_coder)
	{
		dath->dongles[i].id = i;
		dath->dongles[i].is_active = 0;
		dath->dongles[i].ready_at = 0;
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
	if (!dath->coders)
		return(free(dath->coders), 0);

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



int validation(int argc, char* argv[])
{
	int i = 1;
	if (argc != 9)
	{
		printf("ERROR! Has to be exactly 9 parameters!");
		return 0;
	}
	while(i < 8)
	{
		if (!is_valid_num(argv[i]))
		{
			printf("%s > - invalid format", argv[i]);
			return 0;
		}
		i++;
	}
	return 1;
}


int is_valid_num(char *str)
{
    int i = 0;

    if (!str || str[0] == '\0')
        return 0;

    if (str[i] == '+')
        i++;

    if (str[i] == '\0')
        return 0;

    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return 0;
        i++;
    }

    return 1;
}

void fill_fields(int **fields, int count, char **argv)
{
	int i =0;
	while(i < count)
	{
		*fields[i] = atoi(argv[i+1]);
		i++;
	}
}

int main(int argc, char* argv[])
{
	struct data_holder *dh = malloc(sizeof(struct data_holder));
	if (!dh)
		return (1);

	int *fields[] = {
    	&dh->number_of_coder,
    	&dh->time_to_burnout,
    	&dh->time_to_compile,
    	&dh->time_to_debug,
    	&dh->time_to_refractor,
    	&dh->number_of_compiles_required,
    	&dh->dongle_cooldown
	};
	if(validation(argc,argv))
	{
		fill_fields(fields, 7, argv);
		dh->scheduler = argv[8];
	}
	else return (free(dh), 1);
	
	return 0;

}
void *coder_routine(void *arg)
{
	t_coder *coder = (t_coder *)arg;

	while(coder->dh->is_running)
	{
		if(coder->left->is_active && coder->compile_count < coder->dh->number_of_compiles_required)
		{
			printf("%d has taken a dongle", coder->id);
		}
		if(coder->right->is_active && coder->compile_count < coder->dh->number_of_compiles_required)
		{
			printf("%d has taken a dongle", coder->id);
		}
		if(coder->right->is_active && coder->left->is_active)
		{
			coder->compile_count++;
			printf("%d is compiling", coder->id);
		}
	}
}