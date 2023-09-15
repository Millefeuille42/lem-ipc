//
// Created by millefeuille on 9/15/23.
//

#include "lemipc.h"

int game_loop(t_app *app) {
	errno = 0;
	sem_wait(&app->shared->lock);
	if (errno) {
		sem_post(&app->shared->lock);
		return errno;
	}
	sleep(1);
	// TODO do game stuff
	//  if player loses, return `non 0`
	sem_post(&app->shared->lock);
	return 0;
}
