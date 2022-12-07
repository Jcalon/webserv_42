NAME	=	megaphone
SRCS	=	megaphone.cpp
OBJS	=	${SRCS:.cpp=.o}
CC		=	c++
CFLAGS	=	-Wall -Wextra -Werror -std=c++98

all:		${NAME}

%.o:		%.cpp
	${CC} ${CFLAGS} -c $< -o $@


${NAME}:	${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re : fclean all

.PHONY: all fclean clean re


