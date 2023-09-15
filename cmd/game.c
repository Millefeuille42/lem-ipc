//
// Created by millefeuille on 9/15/23.
//

#include "lemipc.h"

t_vec get_first_free(MAP) {
	errno = 0;
	for (unsigned int y = 0; y < BOARD_Y; y++) {
		for (unsigned int x = 0; x < BOARD_Y; x++) {
			if (!map[y][x]) return (t_vec){x, y};
		}
	}
	errno = EUSERS;
	return (t_vec){-1, -1};
}

unsigned int get_diff(unsigned int a, unsigned int b) {
	int diff = (int)(a - b);
	return diff >= 0 ? diff : diff * -1;
}

t_vec get_distance(t_vec *const from, t_vec *const to) {
	return (t_vec) {
		get_diff(from->x, to->x),
		get_diff(from->y, to->y),
	};
}

void move_to(t_player *player, t_vec *const to, MAP) {
	if (to->x >= BOARD_X || to->y >= BOARD_Y || map[to->y][to->x]) return;
	t_vec distance = get_distance(&player->cur_pos, to);
	if (distance.x > 1 || distance.y > 1) return;
	map[player->cur_pos.y][player->cur_pos.x] = 0;
	map[to->y][to->x] = player->team;
	player->cur_pos = *to;
}

int game_loop(t_app *app) {
	errno = 0;
	sem_wait(&app->shared->lock);
	if (errno) {
		sem_post(&app->shared->lock);
		return errno;
	}
	if (!app->player.has_spawned) {
		printf("has not spawned\n");
		t_vec first_free = get_first_free(app->shared->map);
		if (errno) {
			printf("could not register position\n");
			sem_post(&app->shared->lock);
			return 0;
		}
		printf("registered position %u,%u\n", first_free.x, first_free.y);
		app->shared->map[first_free.y][first_free.x] = app->player.team;
		app->player.cur_pos = first_free;
		app->player.has_spawned = 1;
	}

	// Idle movement
	char next_move = (char)(random() % 4);
	switch (next_move) {
		case 0: move_to(&app->player, &(t_vec){app->player.cur_pos.x - 1, app->player.cur_pos.y}, app->shared->map);
			break;
		case 1: move_to(&app->player, &(t_vec){app->player.cur_pos.x + 1, app->player.cur_pos.y}, app->shared->map);
			break;
		case 2: move_to(&app->player, &(t_vec){app->player.cur_pos.x, app->player.cur_pos.y - 1}, app->shared->map);
			break;
		case 3: move_to(&app->player, &(t_vec){app->player.cur_pos.x, app->player.cur_pos.y + 1}, app->shared->map);
			break;
		default: break;
	}
	// TODO do game stuff
	//  if player loses, return `non 0`
	sem_post(&app->shared->lock);
	usleep(TICK_RATE * 1000);
	return 0;
}
