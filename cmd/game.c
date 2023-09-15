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

void move_to(t_app *app, t_vec to) {
	if (to.x < 0 || to.y < 0 || to.x >= BOARD_X || to.y >= BOARD_Y) return;
	int offset = app->cur_pos.x - to.x;
	offset = offset >= 0 ? offset : offset * -1;
	if (offset > 1) return;
	offset = app->cur_pos.y - to.y;
	offset = offset >= 0 ? offset : offset * -1;
	if (offset > 1) return;

	if (app->shared->map[to.y][to.x]) return;
	app->shared->map[app->cur_pos.y][app->cur_pos.x] = 0;
	app->shared->map[to.y][to.x] = app->team;
	app->cur_pos = to;
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

	// Idle movement
	char next_move = (char)(random() % 4);
	switch (next_move) {
		case 0: move_to(app, (t_vec){app->cur_pos.x - 1, app->cur_pos.y}); break;
		case 1: move_to(app, (t_vec){app->cur_pos.x + 1, app->cur_pos.y}); break;
		case 2: move_to(app, (t_vec){app->cur_pos.x, app->cur_pos.y - 1}); break;
		case 3: move_to(app, (t_vec){app->cur_pos.x, app->cur_pos.y + 1}); break;
		default: break;
	}
	// TODO do game stuff
	//  if player loses, return `non 0`
	sem_post(&app->shared->lock);
	usleep(TICK_RATE * 1000);
	return 0;
}
