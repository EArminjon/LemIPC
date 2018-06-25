/*
** EPITECH PROJECT, 2018
** lemipc
** File description:
** semaphor
*/

#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "lemipc.h"

void lockSem(struct sembuf *sops, int sem_id, int value)
{
	sops->sem_op = -1;
	while (semctl(sem_id, 0, GETVAL) == value)
		usleep(30000);
	semop(sem_id, sops, 1);
}

void unlockSem(struct sembuf *sops, int sem_id)
{
	sops->sem_op = 1;
	semop(sem_id, sops, 1);
}

int createSem(key_t key)
{
	int sem_id;

	sem_id = semget(key, 1, SHM_R | SHM_W);
	if (sem_id == -1) {
		sem_id = semget(key, 1, IPC_CREAT | SHM_R | SHM_W);
		semctl(sem_id, 0, SETVAL, 1);
	}
	return (sem_id);
}
