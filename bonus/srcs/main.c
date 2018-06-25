/*
** EPITECH PROJECT, 2018
** lemipc
** File description:
** main
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lemipc.h"

int error_msg(char *msg, int value)
{
	printf("%s", msg);
	return (value);
}

int error(int value)
{
	printf("./lemipc --help\n");
	return (value);
}

int usage(int value)
{
	printf("%s", "USAGE\n       ./lempic PATH TEAM_NUMBER\n\nDESCRIPTION");
	printf("%s", "\n       PATH         path you\'ll give to ftok\n     ");
	printf("%s", "  TEAM_NUMBER  team number of the current champion");
	printf("%s", " (greater than 0)\n");
	return (value);
}

int main(int ac, char **av)
{
	int res = 0;

	if (ac == 3 && av[1] != NULL && av[2] != NULL)
		res = lemipc(av[1], atoi(av[2]));
	else if (ac == 2 && av[1] != NULL && (strcmp(av[1], "--help") == 0))
		res = usage(0);
	else
		res = error(84);
	return (res);
}
