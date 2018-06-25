/*
** EPITECH PROJECT, 2018
** lemipc
** File description:
** is alive
*/

#include <stdio.h>
#include "lemipc.h"

int find_winner(t_player *player, char *map)
{
	int index = 0;

	while (index < MAP_MALLOC) {
		if (map[index] >= '1' && map[index] <= '9' && map[index] != player->id)
			return map[index] - 48;
		++index;
	}
	return (-1);
}

void endGame(t_player *player, t_data *data)
{
	int winner;
	int index = 0;
	char first = '\0';
	char second = '\0';
	char msg[MSG_SIZE];

	while (index <= MAP_MALLOC) {
		if (data->map[index] >= '1' && data->map[index] <= '9') {
			if (first == '\0')
				first = data->map[index];
			else if (second == '\0' && data->map[index] != first)
				second = data->map[index];
		}
		++index;
	}
	if (first == '\0' || second == '\0') {
		lockSem(&player->sops, player->sem_id, 0);
		winner = find_winner(player, data->map);
		sprintf(msg, "game:end;winner:%i", winner);
		set_msg(1, msg, data->msg[0], &player->msg);
		unlockSem(&player->sops, player->sem_id);
	}
}

void check_this(t_position pos, char id, char *map, int *enemies)
{
	int range = 0;
	int xbis;

	while (range < 3) {
		xbis = pos.x + range;
		if (xbis >= 0 && xbis <= MAP_SIZE && pos.y >= 0 && pos.y <= MAP_SIZE &&
			map[CHARPOS(xbis, pos.y)] >= '1' &&
			map[CHARPOS(xbis, pos.y)] <= '9' &&
			map[CHARPOS(xbis, pos.y)] != id) {
			enemies[map[CHARPOS(xbis, pos.y)] - 48] += 1;
		}
		++range;
	}
}

int check_enemies(int *enemies)
{
	int index = 1;

	while (index <= 9) {
		if (enemies[index] >= 2)
			return (2);
		++index;
	}
	return (0);
}

void iAmAlive(t_player *player, t_data *data)
{
	int enemies[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int enemy = 0;

	check_this((t_position) {player->pos.x - 1, player->pos.y - 1}, player->id,
			   data->map, enemies);
	check_this((t_position) {player->pos.x - 1, player->pos.y}, player->id,
			   data->map, enemies);
	check_this((t_position) {player->pos.x - 1, player->pos.y + 1}, player->id,
			   data->map, enemies);
	enemy = check_enemies(enemies);
	if ((enemy == 2) ||
		(end(data->msg[0], &player->sops, player->sem_id) != 0)) {
		player->isAlive = FALSE;
		lockSem(&player->sops, player->sem_id, 0);
		data->map[CHARPOS(player->pos.x, player->pos.y)] = '.';
		unlockSem(&player->sops, player->sem_id);
	}
}