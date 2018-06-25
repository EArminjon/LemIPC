/*
** EPITECH PROJECT, 2018
** lemipc
** File description:
** move player
*/

#include "lemipc.h"
#include <stdio.h>
#include <unistd.h>

int caseIsValid(const char *map, int x, int y)
{
	return (map[CHARPOS(x, y)] == '.' ? 0 : 1);
}

int move_player(t_player *player, char *map)
{
	int mdr = 0;
	int xd = 0;
	static int i = 0;
	if (player->order == -1) {
		return 0;
	}
	lockSem(&player->sops, player->sem_id, 0);
	if (CHARPOS(player->pos.x, player->pos.y) > 0
	    && CHARPOS(player->pos.x, player->pos.y) % (MAP_SIZE + 1) >= player->order % (MAP_SIZE +1)
	    && caseIsValid(map, player->pos.x - 1, player->pos.y) == 0)
		mdr = -1;
	else if (CHARPOS(player->pos.x, player->pos.y) > (MAP_SIZE + 1)
		 && CHARPOS(player->pos.x, player->pos.y) / (MAP_SIZE + 1) > player->order / (MAP_SIZE + 1)
		 && caseIsValid(map, player->pos.x, player->pos.y - 1) == 0)
		xd = -1;
	else if (CHARPOS(player->pos.x, player->pos.y) % (MAP_SIZE + 1) < player->order % (MAP_SIZE + 1)
		 && caseIsValid(map, player->pos.x + 1, player->pos.y) == 0)
		mdr = 1;
	else if (caseIsValid(map, player->pos.x, player->pos.y + 1) == 0)
		xd = 1;
	else if (caseIsValid(map, player->pos.x + 1, player->pos.y) == 0)
		mdr = 1;
	map[CHARPOS(player->pos.x, player->pos.y)] = '.';
	player->pos.x += mdr;
	player->pos.y += xd;
	map[CHARPOS(player->pos.x, player->pos.y)] = player->id;
	++i;
	unlockSem(&player->sops, player->sem_id);
	return (0);
}
