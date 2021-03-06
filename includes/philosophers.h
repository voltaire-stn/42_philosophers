#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <time.h>
# include <sys/time.h>
# include <unistd.h>
# include "color_shell.h"

# define THINKING 0
# define TAKEN_A_FORK 1
# define EATING 2
# define SLEEPING 3
# define DIED 4

# define INIT 0
# define DESTROY 1

typedef struct s_phil
{
	int				nb_phil;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	int				starting_time;
	int				id;
	int				eating_times;
	int				died;
	int				left;
	int				right;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
	pthread_mutex_t	die_and_eat;
	pthread_mutex_t	*lock_print;
	int				curr_time;
	int				last_eating;
	pthread_t		th_phil;
	int				*end;
}					t_phil;

long	ft_atoi_long(char const *s);

void	init_phil(t_phil *phil, int id, char**av,
			pthread_mutex_t **forks);
int		fork_mutexes(int option, pthread_mutex_t *forks, int nb_phil);

int		print_error(char *msg, t_phil *phil);
void	print_rendu(t_phil *phil, int option);
int		change_state_and_print(t_phil **phil, int new_state);

int		get_time(void);
void	millisleep(int to_spend, int curr_time, int starting_time, int *end);
int		ret_current_time(t_phil phil);

int		start_diner(t_phil *phils, int nb_phil);

#endif