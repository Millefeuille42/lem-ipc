//
// Created by millefeuille on 9/12/23.
//

#include "lemipc.h"

char init_key(void) {
	struct stat keyfile_stat;
	stat(KEY_FILE, &keyfile_stat);
	if (errno) {
		if (errno == ENOENT) {
			errno = 0;
			ft_putstr("keyfile not found, creating it...\n");
			int fd = creat(KEY_FILE, S_IREAD | S_IWRITE);
			if (errno) panic("creat");
			close(fd);
			if (errno) panic("close");
			return 1;
		} else panic("stat");
	}
	return 0;
}

int get_key(void) {
	key_t key = ftok(KEY_FILE, PROJ_KEY);
	if (errno) panic("ftok");
	return key;
}

int init_shared(t_app *app) {
	char is_first = init_key();
	int shm_id = shmget(get_key(), sizeof(int) * 4, IPC_CREAT | 0666);
	if (errno) return errno;
	app->shared = shmat(shm_id, NULL, 0);
	if (errno) {
		shmctl(shm_id, IPC_RMID, NULL);
		return errno;
	}
	if (is_first) {
		bzero(app->shared, sizeof(*app->shared));
		app->shared->shm_id = shm_id;
	}
	return is_first;
}

unsigned long get_attached_num(int shm_id) {
	errno = 0;
	struct shmid_ds buf;
	ft_bzero(&buf, sizeof(buf));
	shmctl(shm_id, IPC_STAT, &buf);
	if (errno) return 0;
	return buf.shm_nattch;
}

int quit(t_app *app) {
	if (app->window) mlx_destroy_window(app->mlx, app->window);
	if (app->img.v_img) mlx_destroy_image(app->mlx, app->img.v_img);
	if (app->mlx) {
		app->shared->has_ui = 0;
		mlx_destroy_display(app->mlx);
		free(app->mlx);
	}
	int shm_id = (int)app->shared->shm_id;
	if (app->shared) {
		errno = 0;
		printf("detaching from shared memory %d\n", shm_id);
		shmdt(app->shared);
		if (errno) panic("shmdt");
	}
	unsigned long nattch = get_attached_num(shm_id);
	if (errno) panic("nattch");
	if (nattch <= 0) {
		printf("deleting shared memory %d\n", shm_id);
		shmctl(shm_id, IPC_RMID, NULL);
		if (errno) panic("IPC_RMID");
		remove(KEY_FILE);
	}
	printf("exiting...\n");
	stop_sem = NULL;
	exit(0);
}

void loop(t_app *app) {
	while (1) {
		sleep(1);
		printf("main_loop\n");
		sem_trywait(&app->stop_sem);
		if (!errno || errno == EINTR) break;
		else if (errno && errno != EAGAIN) log_error("sem");
		printf("%d", app->shared->has_ui);
		if (!app->shared->has_ui) {
			app->shared->has_ui = 1;
			printf("creating UI\n");
			ui_start(app);
			stop_sem = NULL;
		}
	}
}

int main(void) {
	// TODO add semaphores for shared mem access
	// TODO add game elements
	// TODO create game loop

	errno = 0;
	srand(time(NULL));
	if (errno) panic("srand");

	t_app app;
	ft_bzero(&app, sizeof(app));
	int is_first = init_shared(&app);
	if (errno) panic("init shared");
	if (is_first) printf("created shared memory%d\n", app.shared->shm_id);
	else printf("got shared memory%d\n", app.shared->shm_id);

	unsigned long nattch = get_attached_num(app.shared->shm_id);
	if (errno) {
		shmctl(app.shared->shm_id, IPC_RMID, NULL);
		return errno;
	}
	printf("%lu process(es) attached\n", nattch);

	sem_init(&app.stop_sem, 0, 0);
	stop_sem = &app.stop_sem;
	setup_sigs();
	if (errno) panic("sigs");
	loop(&app);
	quit(&app);
}
