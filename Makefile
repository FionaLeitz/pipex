_END=$'\033[0m
_GREEN=$'\033[1;32m
_YELLOW=$'\033[1;33m
_ORANGE=$'\033[38;2;255;165;0m

NAME		= pipex

SRCS_DIR	= srcs

SRCS_NAME	= test.c			\

SRCS		= ${addprefix ${SRCS_DIR}/, ${SRCS_NAME}}

OBJ_DIR		= obj

OBJS		= ${addprefix ${OBJ_DIR}/, ${SRCS:.c=.o}}

CC			= gcc

CFLAGS		= -Wall -Wextra -Werror

LIB			= libft/libft.a

INC			= libft/headers/ft_printf.h libft/headers/libft.h

all:		$(NAME)

${NAME}:	${OBJ_DIR} ${OBJS}
			@echo "${_ORANGE}Objects pipex created${_END}"
			@make -C libft
			@${CC} ${CFLAGS} ${OBJS} ${LIB} -o ${NAME}
			@echo "${_GREEN}PIPEX compiled succesfully !${_END}"

${OBJ_DIR}:
			@mkdir -p $@/$(SRCS_DIR)

$(OBJ_DIR)/%.o: %.c
			@${CC} ${CFLAGS} -c $< -o $@

clean:
			@rm -rf $(OBJ_DIR)
			@make clean -C libft
			@echo "${_YELLOW}Objects cleaned !${_END}"

fclean:		clean
			@rm -rf ${NAME}
			@rm -rf ${LIB}
			@echo "${_YELLOW}${NAME} cleaned !${_END}"

re:			fclean all

.PHONY:		clean fclean re all