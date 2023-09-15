//
// Created by millefeuille on 9/15/23.
//

#include "game.h"

t_vec get_first_free(MAP) {
	errno = 0;
#ifdef RANDOM_SPAWN
	for (int tries = 0; tries < 50; tries++) {
		t_vec pos = {random() % BOARD_X, random() % BOARD_Y};
		if (!map[pos.y][pos.x]) return pos;
	}
#else
	for (unsigned int y = 0; y < BOARD_Y; y++) {
		for (unsigned int x = 0; x < BOARD_X; x++) {
			if (!map[y][x]) return (t_vec){x, y};
		}
	}
#endif
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

// FIXME Could be improved (like bookkeeping per team num of players ?)
size_t get_board_info(t_player *player, t_vec *closest_enemy, MAP) {
	size_t count = 0;
	unsigned int min_dist = BOARD_X + BOARD_Y + 1;
	errno = ENOENT;
	for (unsigned int y = 0; y < BOARD_Y; y++) {
		for (unsigned int x = 0; x < BOARD_Y; x++) {
			if (map[y][x] == player->team) count++;
			if (map[y][x] && map[y][x] != player->team) {
				t_vec distance = get_distance(&player->cur_pos, &(t_vec){x, y});
				unsigned int dist = distance.x + distance.y;
				if (dist < min_dist) {
					errno = 0;
					min_dist = dist;
					*closest_enemy = (t_vec){x, y};
				}
			}
		}
	}
	return count;
}

void spawn_behavior(t_app *app) {
	printf("has not spawned\n");
	t_vec first_free = get_first_free(app->shared->map);
	if (errno) {
		errno = 0;
		printf("could not register position\n");
		return;
	}
	printf("registered position %u,%u\n", first_free.x, first_free.y);
	app->shared->map[first_free.y][first_free.x] = app->player.team;
	app->player.cur_pos = first_free;
	app->player.has_spawned = 1;
	idle_behavior(app);
}

void idle_behavior(t_app *app) {
	t_vec closest_enemy = {0};
	unsigned int num_of_mates = get_board_info(&app->player, &closest_enemy, app->shared->map);
	if (!errno && num_of_mates >= 2) {
		app->player.prey = closest_enemy;
		app->player.has_prey = 1;
		hunt_behavior(app);
		return;
	}
	printf("idle\n");

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
	errno = 0;
}

void hunt_behavior(t_app *app) {
	unsigned int prey_value = app->shared->map[app->player.prey.y][app->player.prey.x];
	if (!prey_value || prey_value == app->player.team) {
		app->player.has_prey = 0;
		idle_behavior(app);
		return;
	}
	printf("on the hunt\n");

	t_vec prey_dist = get_distance(&app->player.cur_pos, &app->player.prey);
	if (prey_dist.x > prey_dist.y) {
		int move = (int)(app->player.prey.x - app->player.cur_pos.x) > 0 ? 1 : -1;
		move_to(&app->player, &(t_vec){app->player.cur_pos.x + move, app->player.cur_pos.y}, app->shared->map);
		return;
	}
	int move = (int)(app->player.prey.y - app->player.cur_pos.y) > 0 ? 1 : -1;
	move_to(&app->player, &(t_vec){app->player.cur_pos.x, app->player.cur_pos.y + move}, app->shared->map);
}

unsigned int get_team_of_tile(int y, int x, MAP) {
	if (y >= BOARD_Y || x >= BOARD_X || y < 0 || x < 0) return 0;
	return map[y][x];
}

size_t get_adjacent_of_team(unsigned int team, t_vec *const position, MAP) {
	size_t count = 0;

	for (int x = - 1; x <= 1; x++) {
		for (int y = - 1; y <= 1; y++) {
			int pos_x = (int)(position->x) + x;
			int pos_y = (int)(position->y) + y;
			count = get_team_of_tile(pos_y, pos_x, map) == team ? count + 1 : count;
		}
	}

	return count;
}

int is_surrounded(t_player *player, MAP) {
	for (int x = - 1; x <= 1; x++) {
		for (int y = - 1; y <= 1; y++) {
			int pos_x = (int)(player->cur_pos.x) + x;
			int pos_y = (int)(player->cur_pos.y) + y;
			unsigned int tile_team = get_team_of_tile(pos_y, pos_x, map);
			if (tile_team != 0 && tile_team != player->team && get_adjacent_of_team(tile_team, &player->cur_pos, map) >= 2)
				return 1;
		}
	}
	return 0;
}

int game_loop(t_app *app) {
	errno = 0;
	if (app->observer_team == app->player.team) {
		usleep(TICK_RATE * 1000);
		return 0;
	}
	sem_wait(&app->shared->lock);
	if (errno) {
		sem_post(&app->shared->lock);
		return errno;
	}

	if (is_surrounded(&app->player, app->shared->map)) {
		sem_post(&app->shared->lock);
		return 1;
	}

	if (!app->player.has_spawned) spawn_behavior(app);
	else if(app->player.has_prey) hunt_behavior(app);
	else idle_behavior(app);
	// TODO do real clock instead of fixed sleep (check loop time)
	sem_post(&app->shared->lock);
	usleep(TICK_RATE * 1000);
	return 0;
}
