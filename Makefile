CC = clang
CCFLAGS = -Wall -Wextra -Werror
SOURCE_DIR = src/
SRCS_F = main.c
SRCS = $(addprefix ${SOURCE_DIR}, ${SRCS_F})
ODIR = objs/
OBJS = $(addprefix ${ODIR}, ${SFILES:.c=.o})
LIB_EXE = libft/libft.a
INCL = -I ./includes -I ./libft
LIB_COMPIL = -L./libft -lft
RM = rm -rf



clean:
	${RM} ${ODIR}
	@make -C libft clean
fclean:
	${RM} ${ODIR}
	${RM} ${NAME}
	@make -C libft fclean
re: fclean all

.PHONY:	all clean re fclean