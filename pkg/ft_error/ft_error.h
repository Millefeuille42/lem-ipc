//
// Created by millefeuille on 4/21/23.
//

#ifndef FT_LS_FT_ERROR_H
# define FT_LS_FT_ERROR_H

# include <string.h>
# include <errno.h>
# include <stdlib.h>
#include <stdio.h>

# include <ft_log.h>

# ifndef PROGRAM_NAME
#  define PROGRAM_NAME ""
# endif

void log_error(char *);
void panic(char *);

#endif //FT_LS_FT_ERROR_H
