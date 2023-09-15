//
// Created by millefeuille on 9/14/23.
//

#ifndef LEMIPC_LEMIPC_H
# define LEMIPC_LEMIPC_H

#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <ft_error.h>

#include "ui.h"

#ifndef PROJ_KEY
# define PROJ_KEY 4242
#endif

#ifndef KEY_FILE
# define KEY_FILE "./keyfile.key"
#endif

#ifndef SCREEN_X
# define SCREEN_X 500
#endif

#ifndef SCREEN_Y
# define SCREEN_Y 500
#endif

#ifndef BOARD_X
# define BOARD_X 50
#endif

#ifndef BOARD_Y
# define BOARD_Y 50
#endif

extern sem_t *stop_sem;

typedef struct s_shared {
	int shm_id;
	int has_ui;
	sem_t lock;
	char map[BOARD_Y][BOARD_X];
} t_shared;

typedef struct s_app {
	void *mlx;
	void *window;
	t_img img;
	t_shared *shared;
	sem_t stop_sem;
} t_app;

void ui_start(t_app *app);
int quit(t_app *app);
void handle_signal(int sig, siginfo_t *info, void *context);
void setup_sigs(void);

int get_shmem(int key, size_t size);
void delete_shmem(int shm_id);
void detach_shmem(void **mem);
void *attach_shmem(int shm_id);
unsigned long get_shmem_nattch(int shm_id);
int game_loop(t_app *app);

#endif //LEMIPC_LEMIPC_H
