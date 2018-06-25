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

t_position convert(int pos)
{
	int x = pos % (MAP_SIZE + 1);
	int y = pos / (MAP_SIZE + 1);
	return (t_position) {x, y};
}

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
	int range = 1;

	while (range <= MAP_SIZE) {
		for (int y = xbis - range ; y < ybis + range ; ++y) {
			for (int x = xbis - range ; x < xbis + range ; ++x) {
				int area = CHARPOS(x, y);
				if (y > 0 && y <= MAP_MALLOC && x >= 0 && x <= MAP_SIZE
					&& map[area] >= '1' && map[area] <= '9' && map[area] != player->id)
					return (area);
			}
		}
		++range;
	}
	return (-1);
}

char *parse_order(char *order, char *action)
{
	int i = 0;
	char *str;

	str = strstr(order, action);
	if (str != NULL && action != NULL) {
		while (str[i] == action[i] && action[i] != '\0' && str[i] != '\0')
			++i;
		if (action[i] == '\0' && str[i] != '\0')
			return (&str[i]);
	}
	return (NULL);
}

void set_order(t_player *player, t_position start, char *map)
{
	char *order = get_msg(player->msg_id, &player->msg, player->team);

	if ((order == NULL || order[0] == '\0')) {

		lockSem(&player->sops, player->sem_id, 0);
		int pos = find_target(player, start, map);
		unlockSem(&player->sops, player->sem_id);

		if (pos != -1) {
			char msg[MSG_SIZE];
			sprintf(msg, "%s%i", "target:\0", pos);
			set_msg(player->team, msg, player->msg_id, &player->msg);
		}
	}
}

void get_order(t_player *player, char *map)
{
	char *order = get_msg(player->msg_id, &player->msg, player->team);

	if (order == NULL || order[0] == '\0') {
		player->order = -1;
		return;
	}
	char *data = parse_order(order, "target:");
	if (data == NULL)
		return;
	int pos = atoi(data);
	if (!(map[pos] >= '1' && map[pos] <= '9' && map[pos] != player->id)) {
		remove_msg(player->msg_id, &player->msg, player->team);
		set_order(player, convert(pos), map);
		get_order(player, map);
	}

	player->order = pos;
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
