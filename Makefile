NAME	= WebServ

OBJS_PATH = objs/
DEPS_PATH = deps/

SRCS 	=	main.cpp \
			srcs/Config/Config.cpp \
			srcs/Config/Location.cpp \
			srcs/Config/Server.cpp \
			srcs/Request.cpp \
			srcs/Response.cpp \
			srcs/Server.cpp \
			srcs/Socket.cpp \
			srcs/Manager.cpp \
			srcs/utils.cpp

OBJS 		= ${SRCS:%.cpp=${OBJS_PATH}%.o}
DEPS		= ${SRCS:%.cpp=${DEPS_PATH}%.d}

CXX 		= c++
CXXFLAGS 	= -Wall -Wextra -Werror -std=c++98 -g

INCLUDES = -I includes -I includes/Config

all: 	${NAME}

${OBJS_PATH}%.o: %.cpp ${DEPS_PATH}%.d
		@mkdir -p ${@D}
		${CXX} ${CXXFLAGS} ${INCLUDES} -MMD -MF ${DEPS_PATH}$*.d -c $< -o $@

${DEPS_PATH}%.d: %.cpp
		@mkdir -p ${@D}

${NAME}: Makefile ${OBJS}
		@echo -e "\n----Compiling ft_containers----\n"
		${CXX} ${CXXFLAGS} ${OBJS} ${INCLUDES} -o ${NAME}
		@echo -e "\nft_containers Compiled!\n"

-include ${DEPS}

clean:
		@rm -rf ${OBJS_PATH}
		@rm -rf ${DEPS_PATH}

fclean: clean
		@rm -f ${NAME}
		@echo -e "\nDeleting everything!\n"

re:		fclean
		make all

re_bonus: fclean bonus

.PHONY: all clean fclean re re_bonus bonus
