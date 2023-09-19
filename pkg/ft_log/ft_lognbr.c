//
// Created by millefeuille on 9/19/23.
//

#include "ft_log.h"

void ft_lognbr(enum log_level level, long n) {
	if (level > LOG_LEVEL) return;
	print_level(level);

	ft_putnbr(n);
}
