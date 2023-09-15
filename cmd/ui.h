//
// Created by millefeuille on 9/14/23.
//

#ifndef LEMIPC_UI_H
# define LEMIPC_UI_H

# include <mlx.h>
# include <X11/X.h>

typedef struct s_vec {
	unsigned int x;
	unsigned int y;
} t_vec;

typedef struct s_img {
	void *v_img;
	char *c_img;
	int bpp;
	int sl;
	int endian;
} t_img;

typedef struct s_color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} t_color;

#endif //LEMIPC_UI_H
