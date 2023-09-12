//
// Created by millefeuille on 9/11/23.
//

#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../pkg/ft_error/ft_error.h"

#ifndef PROJ_KEY
# define PROJ_KEY 4242
#endif

#ifndef KEY_FILE
# define KEY_FILE "./keyfile.key"
#endif

char init_key(void) {
	struct stat keyfile_stat;
	stat(KEY_FILE, &keyfile_stat);
	if (errno) {
		if (errno == ENOENT) {
			ft_putstr("keyfile not found, creating it...\n");
			int fd = creat(KEY_FILE, S_IREAD | S_IWRITE);
			if (errno) panic("creat: ");
			close(fd);
			if (errno) panic("close: ");
			return 1;
		} else panic("stat");
	}
	return 0;
}

int get_key(void) {
	key_t key = ftok(KEY_FILE, PROJ_KEY);
	if (errno) panic("ftok: ");
	return key;
}

int main(void) {
	char is_first = init_key();
	(void) is_first;
	int shm_id = shmget(get_key(), sizeof(int) * 4, IPC_CREAT | 0666);
	if (errno) panic("shmget: ");

	struct shmid_ds buf = {.shm_segsz = 0};
	int shm_id_2 = shmctl(shm_id, IPC_STAT, &buf);
	if (errno) {
		shmctl(shm_id, IPC_RMID, &buf);
		panic("IPC_STAT: ");
	}

	shmctl(shm_id, IPC_RMID, &buf);
	if (errno) panic("IPC_RMID: ");
	printf("%d - %d\n", shm_id, shm_id_2);

	return errno;
}
