######### Sources #########

LIB_MLX_DIR = pkg/minilibx-linux
LIB_MLX = $(LIB_MLX_DIR)/libmlx.a

SOURCES	=			pkg/ft_print/ft_putchar.c\
            		pkg/ft_print/ft_putstr.c\
            		pkg/ft_print/ft_putnbr_base_padded.c\
            		pkg/ft_print/ft_putnbr_base.c\
            		pkg/ft_print/ft_putnbr.c\
            		pkg/ft_error/panic.c\
            		pkg/ft_error/log_error.c\
            		pkg/ft_string/ft_string_copy.c\
                    pkg/ft_string/ft_string_concat.c\
                    pkg/ft_string/get_after_n_sep.c\
                    pkg/ft_string/ft_strcmp.c\
                    pkg/ft_string/ft_to_lower.c\
                    pkg/ft_string/ft_strlen.c\
                    pkg/ft_string/ft_is_alnum.c\
                    pkg/ft_string/ft_split.c\
                    pkg/ft_string/ft_is_alpha.c\
                    pkg/ft_string/string.c\
                    pkg/ft_memory/zeroed_malloc.c\
                    pkg/ft_memory/ft_bzero.c\
                    pkg/ft_memory/del_array.c\
                    pkg/ft_memory/safe_free.c\
            		cmd/main.c\
            		cmd/ui.c\
            		cmd/signals.c\
            		cmd/shmem.c\
            		cmd/game.c\

HEADERS	=	pkg/ft_error/ft_error.h\
			pkg/ft_print/ft_print.h\
            pkg/ft_string/ft_string.h\
            pkg/ft_memory/ft_memory.h\
			pkg/minilibx-linux/mlx.h\
			cmd/main.h\

HEADERS_DIRECTORIES	=	cmd/ \
                        pkg/ft_error/ \
                        pkg/ft_print/ \
                        pkg/ft_memory/ \
                        pkg/ft_string/ \
                        $(LIB_MLX_DIR) \

INCLUDES =	$(addprefix -I, $(HEADERS_DIRECTORIES))

######### Details #########

NAME	=	lemipc
SOURCES_EXTENSION = c

######### Compilation #########

COMPILE		=	gcc
DELETE		=	rm -f

MAKE_LIB_MLX = make -C $(LIB_MLX_DIR)

ifdef TICK_RATE
	TICK_RATE_DEFINE := -DTICK_RATE=$(TICK_RATE)
endif

FLAGS		= -Wall -Werror -Wextra -pedantic $(INCLUDES) $(TICK_RATE_DEFINE)
LINK_FLAGS  = -L$(LIB_MLX_DIR) -lmlx -lX11 -lXext

######### Additional Paths #########

vpath	%.h $(HEADERS_DIRECTORIES)

# ################################### #
#        DO NOT ALTER FURTHER!        #
# ################################### #

######### Additional Paths #########

vpath	%.o $(OBJS_DIR)
vpath	%.d $(DEPS_DIR)

######### Implicit Macros #########

OBJS_DIR	= .objs/
DEPS_DIR	= .deps/

OBJS	=	$(addprefix $(OBJS_DIR), $(SOURCES:.$(SOURCES_EXTENSION)=.o))
DEPS	=	$(addprefix $(DEPS_DIR), $(SOURCES:.$(SOURCES_EXTENSION)=.d))

#########  Rules  #########

.DEFAULT_GOAL := $(NAME)

all:	$(OBJS_DIR) $(DEPS_DIR) $(NAME) ## Compile project and dependencies

$(NAME):	$(OBJS) $(LIB_MLX) ## Compile project
			@printf "Linking %s\n" $@
			@$(COMPILE) $(FLAGS) $^ $(LINK_FLAGS) -o $@

mlx: $(LIB_MLX)

$(LIB_MLX):
	@printf "Compiling mlx\n"
	@$(MAKE_LIB_MLX) > /dev/null

clean: clean_deps clean_objs ## Delete object files

fclean:	clean clean_bin mlx_clean ## Delete object files and binary

mlx_clean:
	@printf "Cleaning mlx\n"
	@$(MAKE_LIB_MLX) clean > /dev/null

re:	fclean ## Delete object files and binary, then recompile all
			@make --no-print-directory all
help: ## Print this help
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z_-]+:.*?## / {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}' $(MAKEFILE_LIST)

#########  Sub Rules  #########

objs:	$(OBJS_DIR) $(DEPS_DIR) $(OBJS)

clean_deps:
			@printf "Deleting %s\n" $(DEPS_DIR)
			@$(DELETE) -r $(DEPS_DIR)
clean_objs:
			@printf "Deleting %s\n" $(OBJS_DIR)
			@$(DELETE) -r $(OBJS_DIR)
clean_bin:
			@printf "Deleting %s\n" $(NAME)
			@$(DELETE) $(NAME)

#########  Implicit Rules  #########

$(OBJS_DIR):
					@mkdir -p $(OBJS_DIR)

$(DEPS_DIR):
					@mkdir -p $(DEPS_DIR)

$(OBJS_DIR)%.o:	%.$(SOURCES_EXTENSION)
			@mkdir -p $(OBJS_DIR)$(dir $<)
			@mkdir -p $(DEPS_DIR)$(dir $<)
			@printf "Compiling %s\n" $^
			@$(COMPILE) $(FLAGS) -MMD -MP -MF $(DEPS_DIR)$*.d -c $< -o $@

.PHONY:	all clean fclean re help mlx mlx_clean objs clean_bin clean_deps clean_objs

#########  Includes  #########

-include $(DEPS)
