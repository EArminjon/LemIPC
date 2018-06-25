/*
** EPITECH PROJECT, 2018
** lemipc
** File description:
** move player
*/

#include "lemipc.h"
#include <stdio.h>
#include <unistd.h>

int case_is_valid(const char *map, int x, int y)
{
	return (map[CHARPOS(x, y)] == '.' ? 0 : 1);
}

void check_move(t_player *player, char *map, t_position *new)
{
	if (CHARPOS(player->pos.x, player->pos.y) > 0
		&& CHARPOS(player->pos.x, player->pos.y) % (MAP_SIZE + 1)
		>= player->order % (MAP_SIZE +1)
		&& case_is_valid(map, player->pos.x - 1, player->pos.y) == 0)
			new->x = -1;
	else if (CHARPOS(player->pos.x, player->pos.y) > (MAP_SIZE + 1)
		&& CHARPOS(player->pos.x, player->pos.y) / (MAP_SIZE + 1)
		> player->order / (MAP_SIZE + 1)
		&& case_is_valid(map, player->pos.x, player->pos.y - 1) == 0)
			new->y = -1;
	else if (CHARPOS(player->pos.x, player->pos.y) % (MAP_SIZE + 1)
		< player->order % (MAP_SIZE + 1)
		&& case_is_valid(map, player->pos.x + 1, player->pos.y) == 0)
			new->x = 1;
	else if (case_is_valid(map, player->pos.x, player->pos.y + 1) == 0)
			new->y = 1;
	else if (case_is_valid(map, player->pos.x + 1, player->pos.y) == 0)
			new->x = 1;
}

int move_player(t_player *player, char *map)
{
	t_position new;
	static int i = 0;

	if (player->order == -1)
		return (0);
	new.x = 0;
	new.y = 0;
	lockSem(&player->sops, player->sem_id, 0);
	check_move(player, map, &new);
	map[CHARPOS(player->pos.x, player->pos.y)] = '.';
	player->pos.x += new.x;
	player->pos.y += new.y;
	map[CHARPOS(player->pos.x, player->pos.y)] = player->id;
	unlockSem(&player->sops, player->sem_id);
	++i;
	return (0);
}
