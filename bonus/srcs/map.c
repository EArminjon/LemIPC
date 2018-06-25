/*
** EPITECH PROJECT, 2018
** lemipc
** File description:
** lemipc map
*/

#include <unistd.h>
#include "lemipc.h"

void show_map(char *map, size_t side)
{
	int sep = 0;
	size_t max = ((side + 1) * side) + 1;

	for (size_t index = 0 ; index < side + 2; ++index)
		write(1, "#", 1);
	write(1, "\n", 1);
	for (size_t index = 0 ; index < max; ++index) {
		if ((sep == 0 && map[index] != '\0') || map[index] == '\n') {
			write(1, "#", 1);
			sep = (sep == 0 ? 1 : 0);
		}
		write(1, &map[index], 1);
	}
	for (size_t index = 0 ; index < side + 2; ++index)
		write(1, "#", 1);
	write(1, "\n", 1);
}

char *createMap(char *map, size_t side)
{
	size_t index = 0;
	size_t max = side * side;

	while (index < max) {
		for (size_t col = 0 ; col < side ; ++col)
			map[index++] = '.';
		map[index++] = '\n';
	}
	map[index] = '\0';
	return map;
}
