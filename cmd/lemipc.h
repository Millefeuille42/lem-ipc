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
#include <ft_string.h>

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
# define BOARD_X 100
#endif

#ifndef BOARD_Y
# define BOARD_Y 100
#endif

#ifndef TICK_RATE
# define TICK_RATE 100
#endif

#ifndef OBSERVER_TEAM
# define OBSERVER_TEAM "observer"
#endif

#define MAP unsigned int map[BOARD_Y][BOARD_X]

extern sem_t *stop_sem;

typedef struct s_shared {
	int shm_id;
	int has_ui;
	t_color color_offsets;
	sem_t lock;
	MAP;
} t_shared;

typedef struct s_player {
	int has_spawned;
	int has_prey;
	unsigned int team;
	t_vec prey;
	t_vec cur_pos;
} t_player;

typedef struct s_app {
	void *mlx;
	void *window;
	t_shared *shared;
	unsigned int observer_team;
	t_player player;
	sem_t stop_sem;
	t_img img;
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
