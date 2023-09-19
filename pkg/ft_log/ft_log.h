//
// Created by millefeuille on 9/19/23.
//

#ifndef LEMIPC_FT_LOG_H
# define LEMIPC_FT_LOG_H

#include <ft_print.h>

enum log_level {
	NONE = 0,
	ERROR,
	INFO,
	DEBUG,
	ALL
};

# ifndef LOG_LEVEL
#  define LOG_LEVEL ERROR
# endif

void print_level(enum log_level level);
void ft_logstr(enum log_level level, char *str);
void ft_logstr_no_header(enum log_level level, char *str);
void ft_lognbr(enum log_level level, long n);
void ft_lognbr_in_between(enum log_level level, char *prefix, long n, char *suffix, char no_header);

#endif //LEMIPC_FT_LOG_H
