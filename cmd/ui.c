//
// Created by millefeuille on 9/14/23.
//

#include "lemipc.h"

inline static void image_pixel_put(const t_vec position, t_img *img, t_color *const color) {
	if (position.x > SCREEN_X || position.y > SCREEN_Y)
		return;
	img->c_img[position.y * img->sl + position.x * (img->bpp / 8)] = (char)color->b;
	img->c_img[position.y * img->sl + position.x * (img->bpp / 8) + 1] = (char)color->g;
	img->c_img[position.y * img->sl + position.x * (img->bpp / 8) + 2] = (char)color->r;
}

inline static void draw_square(t_vec position, t_img *img, t_color *const color) {
	for (unsigned long y = 0; y < (SCREEN_Y / BOARD_Y); y++) {
		for (unsigned long x = 0; x < (SCREEN_X / BOARD_X); x++) {
			image_pixel_put(
					(t_vec){
							position.x * (SCREEN_X / BOARD_X) + x,
							position.y * (SCREEN_Y / BOARD_Y) + y
					}, img, color
			);
		}
	}
}

inline static void hash_number(unsigned int number, t_color *color, t_color *offset) {
	if (!offset->r) offset->r = random() % 255;
	if (!offset->g) offset->g = random() % 255;
	if (!offset->b) offset->b = random() % 255;
	color->r = number * offset->r % 255;
	color->g = number * offset->g % 255;
	color->b = number * offset->b % 255;
}

inline static void draw_board(t_app *app) {
	for (unsigned long y = 0; y < BOARD_Y; y++) {
		for (unsigned long x = 0; x < BOARD_X; x++) {
			t_color color;
			hash_number(app->shared->map[y][x], &color, &app->shared->color_offsets);
			draw_square((t_vec){x, y}, &app->img, &color);
		}
	}
}

inline static int ui_loop(t_app *app) {
	int ret = game_loop(app);
	if (errno) log_error("game_loop");
	if (!app->img.v_img) return 1;
	sem_wait(&app->shared->lock);
	draw_board(app);
	sem_post(&app->shared->lock);
	mlx_put_image_to_window(app->mlx, app->window, app->img.v_img, 0, 0);
	errno = 0;
	sem_trywait(&app->stop_sem);
	if (!errno || errno == EINTR) quit(app);
	else if (errno && errno != EAGAIN) log_error("sem");
	if (ret) quit(app);
	return 0;
}

inline static void create_image(void *mlx, t_img *img) {
	if (!img) return;
	if (img->v_img)
		mlx_destroy_image(mlx, img->v_img);
	img->v_img = mlx_new_image(mlx, SCREEN_X, SCREEN_Y);
	img->c_img = mlx_get_data_addr(img->v_img, &img->bpp, &img->sl, &img->endian);
}

inline static int setup_window(t_app *app) {
	app->mlx = mlx_init();
	if (!app->mlx) return 1;
	app->window = mlx_new_window(app->mlx, SCREEN_X, SCREEN_Y, "lem-ipc");
	if (!app->window) {
		mlx_destroy_display(app->mlx);
		return 1;
	}
	app->img.v_img = NULL;
	create_image(app->mlx, &app->img);
	mlx_hook(app->window, DestroyNotify, StructureNotifyMask, &quit, app);
	mlx_hook(app->window, VisibilityNotify, VisibilityChangeMask, &ui_loop, app);
	mlx_loop_hook(app->mlx, &ui_loop, app);
	return 0;
}

void ui_start(t_app *app) {
	if (setup_window(app)) panic("mlx");
	mlx_loop(app->mlx);
	quit(app);
}
