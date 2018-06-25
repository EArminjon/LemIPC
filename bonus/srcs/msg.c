/*
** EPITECH PROJECT, 2018
** lemipc
** File description:
** msg
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include "lemipc.h"

void remove_msg(int msg_id, t_msg *msg, int team)
{
	bzero(msg, sizeof(*msg));
	msgrcv(msg_id, msg, sizeof(*msg), team, IPC_NOWAIT);
}

int get_this_msg(int msg_id, t_msg *msg, int team, char *request)
{
	bzero(msg, sizeof(*msg));
	msgrcv(msg_id, msg, sizeof(*msg), team, IPC_NOWAIT | 040000);
	return (strcmp(msg->str, request) == 0 ? 0 : 1);
}

char *get_msg(int msg_id, t_msg *msg, int team)
{
	team = team;
	bzero(msg, sizeof(*msg));
	msgrcv(msg_id, msg, sizeof(*msg), 0, IPC_NOWAIT | 040000);
	return (msg->str);
}

void set_msg(int team, char *to_send, int msg_id, t_msg *msg)
{
	bzero(msg, sizeof(*msg));
	msg->mtype = team;
	msg->str[0] = '\0';
	sprintf(msg->str, to_send);
	msgsnd(msg_id, msg, sizeof(*msg), 040000);
}

void createMsg(int key, t_data *data)
{
	int msg_id;

	key = key;
	for (int i = 0; i <= MSG_TEAM_MAX; ++i) {
		msg_id = msgget(IPC_PRIVATE, IPC_CREAT | SHM_R | SHM_W);
		if (msg_id == -1)
			printf("msg[%i]: fail\n", i);
		data->msg[i] = msg_id;
	}
}
