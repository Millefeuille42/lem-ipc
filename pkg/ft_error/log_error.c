//
// Created by millefeuille on 4/21/23.
//

#include <stdio.h>
#include "ft_error.h"

void log_error(char *add) {
    ft_fputstr("lemipc: error: ", 1);
	perror(add);
	ft_putchar('\n');
}
