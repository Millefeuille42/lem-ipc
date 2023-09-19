//
// Created by millefeuille on 4/21/23.
//

#include "ft_error.h"

void log_error(char *add) {
    ft_logstr(ERROR, PROGRAM_NAME);
	ft_logstr_no_header(ERROR, ": error: ");
	perror(add);
}
