//
// Created by millefeuille on 4/21/23.
//

#include "ft_error.h"

void log_error(char *add) {
    ft_fputstr("ft_nm: error: ", 1);
	ft_fputstr(add, 1);
	switch (errno) {
		case ENOENT:
			ft_fputstr(" no such file", 1);
			break;
		case EINVAL:
			ft_fputstr(" file format not recognized", 1);
			break;
		default:
			ft_fputstr(" error", 1);
			break;
	}
	ft_putchar('\n');
}
