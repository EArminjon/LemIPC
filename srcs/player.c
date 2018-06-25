/*
** EPITECH PROJECT, 2018
** lemipc
** File description:
** player
*/

#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include "lemipc.h"

int setStartPosition(t_player *player, char *map)
{
	int x = player->pos.x;
	int y = player->pos.y;
	int max = MAP_SIZE;
	int min = 0;
	size_t index = 0;

	while (index < MAP_MALLOC && map[index] != '.')
		++index;
	if (map[index] != '.')
		return (error_msg("No space found !", 84));
	srand(time(NULL));
	while (map[CHARPOS(x, y)] != '.') {
		x = rand() % (max - min + 1) + min;
		y = rand() % (max - min + 1) + min;
	}
	player->pos.x = x;
	player->pos.y = y;
	map[CHARPOS(x, y)] = player->id;
	return (0);
}

int find_target(t_player *player, t_position start, char *map)
{
	int xbis = start.x;
	int ybis = start.y;
	int area;

	for (int range = 1 ; range <= MAP_SIZE ; ++range) {
		for (int y = xbis - range ; y < ybis + range ; ++y) {
			for (int x = xbis - range ; x < xbis + range ; ++x) {
			area = CHARPOS(x, y);
			if (y > 0 && y <= MAP_MALLOC && x >= 0 &&
			x <= MAP_SIZE
			&& map[area] >= '1' && map[area] <= '9'
			&& map[area] != player->id)
			return (area);
			}
		}
	}
	return (-1);
}

int player_game(t_player *player, t_data *data)
{
	lockSem(&player->sops, player->sem_id, 0);
	if (setStartPosition(player, data->map) == 84)
		return (84);
	unlockSem(&player->sops, player->sem_id);
	usleep(30000);
	iAmAlive(player, data);
	while (player->isAlive == TRUE) {
		set_order(player, player->pos, data->map);
		get_order(player, data->map);
		move_player(player, data->map);
		usleep(30000);
		iAmAlive(player, data);
	}
	endGame(player, data);
	return (0);
}

int create_player(t_data *data, t_id *id, int team_number)
{
	struct sembuf sops;
	t_player player;

	sops.sem_num = 0;
	sops.sem_flg = IPC_NOWAIT;
	sops.sem_op = -1;
	player.key = id->key;
	player.pos.x = 10;
	player.pos.y = 0;
	player.isAlive = TRUE;
	player.sops = sops;
	player.msg.str[0] = '\0';
	player.sem_id = id->sem_id;
	player.msg_id = data->msg[team_number];
	player.team = team_number;
	player.id = (char) (team_number + 48);
	player.order = -1;
	return (player_game(&player, data));
}
