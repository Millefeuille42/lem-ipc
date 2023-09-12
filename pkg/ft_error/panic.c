//
// Created by millefeuille on 4/21/23.
//

#include "ft_error.h"

void panic(char *add) {
    log_error(add);
    exit(errno);
}
