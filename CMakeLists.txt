cmake_minimum_required(VERSION 3.10)
project(lemipc C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall -Werror -Wextra -pedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Werror -Wextra -pedantic")

set(SOURCE_LEM_IPC
		cmd/main.c

		pkg/ft_print/ft_putchar.c
		pkg/ft_print/ft_putstr.c
		pkg/ft_print/ft_putnbr_base_padded.c
		pkg/ft_print/ft_putnbr_base.c
		pkg/ft_print/ft_putnbr.c
		pkg/ft_print/ft_print.h

		pkg/ft_error/panic.c
		pkg/ft_error/log_error.c
		pkg/ft_error/ft_error.h
)

add_executable(lemipc ${SOURCE_LEM_IPC})
target_link_libraries(lemipc LINK_PUBLIC)