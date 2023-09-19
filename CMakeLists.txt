cmake_minimum_required(VERSION 3.10)
project(lemipc C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall -Werror -Wextra -pedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Werror -Wextra -pedantic")

include_directories(./pkg/minilibx-linux)
include_directories(./pkg/ft_error)
include_directories(./pkg/ft_print)
include_directories(./pkg/ft_string)
include_directories(./pkg/ft_memory)

set(SOURCE_LEM_IPC
		pkg/ft_print/ft_putchar.c
		pkg/ft_print/ft_putstr.c
		pkg/ft_print/ft_putnbr_base_padded.c
		pkg/ft_print/ft_putnbr_base.c
		pkg/ft_print/ft_putnbr.c
		pkg/ft_print/ft_print.h

		pkg/ft_error/panic.c
		pkg/ft_error/log_error.c
		pkg/ft_error/ft_error.h

		pkg/ft_memory/zeroed_malloc.c
        pkg/ft_memory/ft_bzero.c
        pkg/ft_memory/del_array.c
        pkg/ft_memory/safe_free.c
		pkg/ft_memory/ft_memory.h

		pkg/ft_string/ft_string_copy.c
		pkg/ft_string/ft_string_concat.c
		pkg/ft_string/get_after_n_sep.c
		pkg/ft_string/ft_strcmp.c
		pkg/ft_string/ft_to_lower.c
		pkg/ft_string/ft_strlen.c
		pkg/ft_string/ft_is_alnum.c
		pkg/ft_string/ft_split.c
		pkg/ft_string/ft_is_alpha.c
		pkg/ft_string/string.c
		pkg/ft_string/ft_string.h

		pkg/minilibx-linux/mlx.h

		cmd/main.c
		cmd/ui.c
        cmd/signals.c
		cmd/shmem.c
		cmd/game.c
		cmd/ui.h
		cmd/lemipc.h
		cmd/game.h
		cmd/msgq.c
		pkg/ft_print/ft_putnbr_in_between.c
)

add_executable(lemipc ${SOURCE_LEM_IPC})
target_link_libraries(lemipc LINK_PUBLIC ${CMAKE_SOURCE_DIR}/pkg/minilibx-linux/libmlx.a libX11.so libXext.so)