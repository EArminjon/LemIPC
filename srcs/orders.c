/*
** EPITECH PROJECT, 2018
** order
** File description:
** order
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

char *parse_order(char *order, char *action)
{
	int i = 0;
	char *str;

	str = strstr(order, action);
	if (str != NULL && action != NULL) {
		while (str[i] == action[i] && action[i] != '\0'
		&& str[i] != '\0')
			++i;
		if (action[i] == '\0' && str[i] != '\0')
			return (&str[i]);
	}
	return (NULL);
}

void set_order(t_player *play, t_position start, char *map)
{
	char *order = get_msg(play->msg_id, &play->msg, play->team);
	char msg[32];
	int pos;

	if ((order == NULL || order[0] == '\0')) {
		lockSem(&play->sops, play->sem_id, 0);
		pos = find_target(play, start, map);
		unlockSem(&play->sops, play->sem_id);
		if (pos != -1) {
			sprintf(msg, "%s%i", "target:\0", pos);
			set_msg(play->team, msg, play->msg_id, &play->msg);
		}
	}
}

void get_order(t_player *player, char *map)
{
	char *order = get_msg(player->msg_id, &player->msg, player->team);
	char *data;
	int pos;

	if (order == NULL || order[0] == '\0') {
		player->order = -1;
		return ;
	}
	data = parse_order(order, "target:");
	if (data == NULL)
		return ;
	pos = atoi(data);
	if (!(map[pos] >= '1' && map[pos] <= '9' && map[pos] != player->id)) {
		remove_msg(player->msg_id, &player->msg, player->team);
		set_order(player, convert(pos), map);
		get_order(player, map);
	}
	player->order = pos;
}
