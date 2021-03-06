#include "philosophers.h"

int	check_death(t_phil *phil)
{
	if (ret_current_time(*phil) - phil->last_eating > phil->time_to_die)
	{
		pthread_mutex_lock(&phil->die_and_eat);
		change_state_and_print(&phil, DIED);
		pthread_mutex_unlock(&phil->die_and_eat);
		pthread_mutex_unlock(phil->fork_left);
		pthread_mutex_unlock(phil->fork_right);
		return (1);
	}
	return (0);
}

int	eats(t_phil *curr)
{
	if (check_death(curr))
		return (1);
	pthread_mutex_lock(curr->fork_right);
	change_state_and_print(&curr, TAKEN_A_FORK);
	pthread_mutex_lock(curr->fork_left);
	change_state_and_print(&curr, TAKEN_A_FORK);
	pthread_mutex_lock(&curr->die_and_eat);
	change_state_and_print(&curr, EATING);
	pthread_mutex_unlock(&curr->die_and_eat);
	millisleep(curr->time_to_eat, curr->curr_time,
		curr->starting_time, curr->end);
	if (curr->eating_times > 0)
		curr->eating_times--;
	pthread_mutex_unlock(curr->fork_left);
	pthread_mutex_unlock(curr->fork_right);
	return (0);
}

void	*routine(void *phil)
{
	t_phil	*curr;

	curr = ((t_phil *)phil);
	if (!(curr->id % 2))
		usleep(200);
	while (1)
	{
		if (eats(curr))
			break ;
		change_state_and_print(&curr, SLEEPING);
		millisleep(curr->time_to_sleep, curr->curr_time,
			curr->starting_time, curr->end);
		if (*curr->end || curr->eating_times == 0)
			break ;
		change_state_and_print(&curr, THINKING);
	}
	pthread_mutex_destroy(&curr->die_and_eat);
	return (phil);
}

void	*monitor(void *phils)
{
	t_phil	*curr;
	int		i;

	curr = ((t_phil *)phils);
	while (1)
	{
		i = -1;
		while (++i < curr->nb_phil)
		{
			if (curr[i].eating_times == 0)
				return (phils);
			if (check_death(&curr[i]))
				return (phils);
			if (*curr[i].end)
				return (phils);
		}
	}
	return (phils);
}

int	start_diner(t_phil *phils, int nb_phil)
{
	void			*phil;
	int				i;
	int				end;
	pthread_mutex_t	lock_print;
	pthread_t		th_monitor;

	end = 0;
	i = -1;
	pthread_mutex_init(&lock_print, NULL);
	while (++i < nb_phil)
	{
		phils[i].end = &end;
		phils[i].lock_print = &lock_print;
		phil = (void *) &phils[i];
		if (pthread_create(&(phils[i].th_phil), NULL, &routine, phil) != 0)
			return (print_error("Failed to create thread", NULL));
	}
	pthread_create(&th_monitor, NULL, &monitor, phils);
	pthread_join(th_monitor, NULL);
	i = -1;
	while (++i < nb_phil)
		pthread_join(phils[i].th_phil, NULL);
	pthread_mutex_destroy(&lock_print);
	return (0);
}
