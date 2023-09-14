//
// Created by millefeuille on 9/14/23.
//

#include "lemipc.h"

sem_t *stop_sem = NULL;

void handle_signal(int sig, siginfo_t *info, void *context) {
	(void)sig;
	(void)context;
	(void)info;
	printf("caught ctrl-c\n");
	sem_post(stop_sem);
}

void setup_sigs(void) {
	struct sigaction sigint;

	sigint.sa_sigaction = handle_signal;
	sigaction(SIGINT, &sigint, NULL);
}
