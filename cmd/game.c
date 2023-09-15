//
// Created by millefeuille on 9/15/23.
//

#include "lemipc.h"

t_vec get_first_free(unsigned int map[BOARD_Y][BOARD_X]) {
	int y;
	for (y = 0; y < BOARD_Y; y++) {
		int x;
		for (x = 0; x < BOARD_Y; x++) {
			if (!map[y][x]) return (t_vec){x, y};
		}
	}
	return (t_vec){-1, -1};
}

int game_loop(t_app *app) {
	errno = 0;
	sem_wait(&app->shared->lock);
	if (errno) {
		sem_post(&app->shared->lock);
		return errno;
	}
	if (!app->has_spawned) {
		printf("has not spawned\n");
		t_vec first_free = get_first_free(app->shared->map);
		if (first_free.x < 0) {
			printf("could not register position\n");
			sem_post(&app->shared->lock);
			return 0;
		}
		printf("registered position %d,%d\n", first_free.x, first_free.y);
		app->shared->map[first_free.y][first_free.x] = app->team;
		app->cur_pos = first_free;
		app->has_spawned = 1;
	}
	// TODO do game stuff
	//  if player loses, return `non 0`
	sem_post(&app->shared->lock);
	return 0;
}
