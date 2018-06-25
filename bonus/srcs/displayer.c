/*
** EPITECH PROJECT, 2018
** lemipc
** File description:
** displayer, print ncurses, sfml what do you want
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include "lemipc.h"

int end(int msg_id, struct sembuf *sops, int sem_id)
{
	t_msg message;
	char *order;
	char *result = NULL;

	lockSem(sops, sem_id, 0);
	order = get_msg(msg_id, &message, 0);
	unlockSem(sops, sem_id);
	if (order == NULL || order[0] == '\0')
		return (0);
	result = parse_order(order, "game:");
	if (strncmp(result, "end", 3) == 0)
		return (atoi(parse_order(result, "winner:")));
	return (0);
}

void wait_destruction_process(char *map)
{
	int index;
	int loop = 0;

	while (loop == 0) {
		loop = 1;
		index = 0;
		while (map[index] != '\0' && index <= MAP_MALLOC) {
			if (map[index] >= '1' && map[index] <= '9')
				loop = 0;
			++index;
		}
		usleep(30000);
	}
}

void host(t_id *id, t_data *data, struct sembuf *sops)
{
	int team = 0;

	while (team == 0) {
		team = end(data->msg[0], sops, id->sem_id);
		if (team != 0)
			break ;
		if (manage_event() == TRUE)
			return ;
		clear_window();
		draw_arena();
		draw_array(data->map);
		refresh_window();
		usleep(30000);
	}
	printf("The winner team is: %i\n", team);
	wait_destruction_process(data->map);
}

int game_loop(t_id *id, t_data *data)
{
	struct sembuf sops;

	sops.sem_num = 0;
	sops.sem_flg = IPC_NOWAIT;
	sops.sem_op = -1;
	init_window();
	host(id, data, &sops);
	shmctl(id->shm_id, IPC_RMID, NULL);
	semctl(id->sem_id, IPC_RMID, 0, NULL);
	for (int i = 0; i <= MSG_TEAM_MAX; ++i)
		msgctl(data->msg[i], IPC_RMID, NULL);
	return (0);
}
