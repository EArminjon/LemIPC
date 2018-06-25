/*
** EPITECH PROJECT, 2018
** lemipc
** File description:
** lemipc function main
*/

#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "lemipc.h"

int insertMap(key_t key, size_t side)
{
	void *addr;
	int shm_id;
	t_data *data;

	shm_id = shmget(key, sizeof(*data), IPC_CREAT | SHM_R | SHM_W);
	addr = shmat(shm_id, NULL, SHM_W);
	data = (t_data *)addr;
	createMap(data->map, side);
	if (data->map == NULL)
		return (error_msg("MALLOC FAIL\n", -1));
	return (shm_id);
}

int manage_arguments(char *path, int team_number)
{
	key_t key;

	if (team_number <= 0 || team_number > 9)
		return (usage(-1));
	key = ftok(path, 0);
	if (key == - 1)
		return (error_msg("Invalid path\n", 84));
	return (key);
}

int lemipc(char *path, int team_number)
{
	t_id id;
	t_data *data;
	void *addr;

	id.key = manage_arguments(path, team_number);
	if (id.key == -1)
		return (84);
	id.shm_id = shmget(id.key, MAP_MALLOC, SHM_R | SHM_W);
	id.sem_id = createSem(id.key);
	if (id.shm_id == -1) {
		id.shm_id = insertMap(id.key, MAP_SIZE);
		if (id.shm_id == -1)
			return (84);
		addr = shmat(id.shm_id, NULL, SHM_R);
		data = (t_data *)addr;
		createMsg(id.key, data);
		return (game_loop(&id, data));
	}
	addr = shmat(id.shm_id, NULL, SHM_W | SHM_R);
	data = (t_data *)addr;
	return (create_player(data, &id, team_number));
}
