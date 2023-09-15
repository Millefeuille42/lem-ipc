//
// Created by millefeuille on 9/12/23.
//

#include "lemipc.h"

inline static char init_key(void) {
	struct stat keyfile_stat;
	stat(KEY_FILE, &keyfile_stat);
	if (errno) {
		if (errno == ENOENT) {
			errno = 0;
			ft_putstr("keyfile not found, creating it...\n");
			int fd = creat(KEY_FILE, S_IREAD | S_IWRITE);
			if (errno) log_error("creat");
			close(fd);
			if (errno) log_error("close");
			return 1;
		} else panic("stat");
	}
	return 0;
}

inline static int get_key(void) {
	key_t key = ftok(KEY_FILE, PROJ_KEY);
	if (errno) panic("ftok");
	return key;
}

inline static int init_shared(t_app *app) {
	char is_first = init_key();
	int shm_id = get_shmem(get_key(), sizeof(t_shared));
	if (errno) return errno;
	app->shared = attach_shmem(shm_id);
	if (errno) {
		delete_shmem(shm_id);
		return errno;
	}
	if (is_first) {
		*app->shared = (t_shared){0};
		sem_init(&app->shared->lock, 1, 1);
		if (errno) {
			delete_shmem(shm_id);
			return errno;
		}
		app->shared->shm_id = shm_id;
	}
	return is_first;
}

void close_lock(sem_t *lock) {
	errno = 0;
	sem_destroy(lock);
	if (errno) log_error("sem_destroy");
}

int quit(t_app *app) {
	if (app->player.has_spawned) {
		sem_wait(&app->shared->lock);
		printf("unregistered\n");
		app->shared->map[app->player.cur_pos.y][app->player.cur_pos.x] = 0;
		sem_post(&app->shared->lock);
	}
	if (app->window) mlx_destroy_window(app->mlx, app->window);
	if (app->img.v_img) mlx_destroy_image(app->mlx, app->img.v_img);
	if (app->mlx) {
		app->shared->has_ui = 0;
		mlx_destroy_display(app->mlx);
		free(app->mlx);
	}
	printf("deleting stop lock\n");
	close_lock(&app->stop_sem);
	int shm_id = (int)app->shared->shm_id;
	if (app->shared) {
		printf("detaching from shared memory %d\n", shm_id);
		detach_shmem((void **) &app->shared);
		if (errno) log_error("shmdt");
	}
	unsigned long nattch = get_shmem_nattch(shm_id);
	if (errno) log_error("nattch");
	if (nattch <= 0) {
		printf("deleting shared lock\n");
		close_lock(&app->shared->lock);
		printf("deleting shared memory %d\n", shm_id);
		delete_shmem(shm_id);
		if (errno) log_error("IPC_RMID");
		remove(KEY_FILE);
	}
	printf("exiting...\n");
	stop_sem = NULL;
	exit(errno);
}

inline static void log_and_quit(t_app *app, char *message) {
	log_error(message);
	quit(app);
}

inline static void loop(t_app *app) {
	while (1) {
		sem_trywait(&app->stop_sem);
		if (!errno || errno == EINTR) break;
		else if (errno && errno != EAGAIN) log_error("sem");
		if (!app->shared->has_ui) {
			app->shared->has_ui = 1;
			printf("creating UI\n");
			ui_start(app);
			stop_sem = NULL;
		}
		int ret = game_loop(app);
		if (errno) log_error("game_loop");
		if (ret) break;
	}
}

unsigned int bj2_hash(char *team) {
	unsigned int hash = 0x811c9dc5;
	unsigned int prime = 0x1000193;

	for (size_t i = 0; i < ft_strlen(team); ++i) {
		unsigned char value = team[i];
		hash = hash ^ value;
		hash *= prime;
	}
	return hash + 1;
}

int main(int argc, char *argv[]) {
	// TODO add game elements
	// TODO create game loop

	if (argc < 2) {
		ft_fputstr("usage: lemipc <team name>\n", 2);
		return EINVAL;
	}

	if (ft_strlen(argv[1]) > 16) {
		ft_fputstr("team name has to be 16 characters max\n", 2);
		return EINVAL;
	}

	errno = 0;
	srandom(time(NULL));
	if (errno) panic("srand");

	t_app app = {0};
	app.player.team = bj2_hash(argv[1]);
	int is_first = init_shared(&app);
	if (errno) {
		if (is_first) remove(KEY_FILE);
		panic("init shared");
	}

	if (is_first) printf("created shared memory %d\n", app.shared->shm_id);
	else printf("got shared memory %d\n", app.shared->shm_id);

	sem_init(&app.stop_sem, 0, 0);
	stop_sem = &app.stop_sem;
	if (errno) log_and_quit(&app, "stop_sem");

	unsigned long nattch = get_shmem_nattch(app.shared->shm_id);
	if (errno) log_and_quit(&app, "nattch");
	printf("%lu process(es) attached\n", nattch);

	setup_sigs();
	if (errno) log_and_quit(&app, "sigs");
	loop(&app);
	quit(&app);
}
