//
// Created by millefeuille on 9/15/23.
//

#include "lemipc.h"

int get_shmem(int key, size_t size) {
	errno = 0;
	return shmget(key, size, IPC_CREAT | 0666);
}

void delete_shmem(int shm_id) {
	errno = 0;
	shmctl(shm_id, IPC_RMID, NULL);
}

void detach_shmem(void **mem) {
	errno = 0;
	shmdt(*mem);
}

void *attach_shmem(int shm_id) {
	errno = 0;
	return shmat(shm_id, NULL, 0);
}

unsigned long get_shmem_nattch(int shm_id) {
	errno = 0;
	struct shmid_ds buf = {0};
	shmctl(shm_id, IPC_STAT, &buf);
	if (errno) return 0;
	return buf.shm_nattch;
}
