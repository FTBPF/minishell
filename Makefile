NAME = minishell

CC = cc

RM = rm -rf

CFLAGS = -Wall -Wextra -Werror -g

SRC = main.c main2.c input_sanitize.c processes.c processes2.c processes3.c processes4.c processes5.c split_cmds.c split_cmds2.c split_cmds3.c utils.c utils2.c built_ins.c built_ins2.c \
		built_ins3.c built_ins4.c built_ins5.c built_ins6.c built_ins7.c built_ins8.c built_ins9.c built_ins10.c utils3.c utils4.c utils5.c here_doc.c here_doc2.c var_expander.c \
		var_expander2.c utils6.c utils7.c

SRC_GNL = get_next_line.c get_next_line_utils.c

SRC_GNL = get_next_line.c get_next_line_utils.c

OBJ = $(SRC:.c=.o)

OBJ_GNL = $(SRC_GNL:.c=.o)

OBJ_GNL = $(SRC_GNL:.c=.o)

all: $(NAME)

$(NAME): $(addprefix SRC/,$(OBJ)) $(addprefix GNL/,$(OBJ_GNL)) $(addprefix GNL/,$(OBJ_GNL))
	@make -s -C ft_printf
	@make -s -C libft
	@$(CC) $(addprefix SRC/,$(OBJ)) $(addprefix GNL/,$(OBJ_GNL)) ft_printf/libftprintf.a libft/libft.a -o $(NAME) -lreadline

clean: 
	@make clean -s -C ft_printf
	@make clean -s -C libft
	@$(RM) $(addprefix SRC/,$(OBJ))
	@$(RM) $(addprefix GNL/,$(OBJ_GNL))

fclean: clean
	@make fclean -s -C ft_printf
	@make fclean -s -C libft
	@$(RM) $(NAME) $(LIB)

re: fclean all

.PHONY: all clean fclean re