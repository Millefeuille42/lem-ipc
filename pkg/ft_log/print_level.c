//
// Created by millefeuille on 9/19/23.
//

#include "ft_log.h"

void print_level(enum log_level level) {
	if (level > LOG_LEVEL) return;

	static char level_message[ALL][12] = {
			"[NONE]  -- \0",
			"[ERROR] -- \0",
			"[INFO]  -- \0",
			"[DEBUG] -- \0",
	};
	ft_putstr(level_message[level]);
}
