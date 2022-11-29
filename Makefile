NAME	= WebServ

SRCS_PATH = srcs/
OBJS_PATH = objs/
DEPS_PATH = deps/

SRCS 	=	main.cpp \
			Config/Config.cpp \
			Config/Location.cpp \
			Config/Server.cpp \
			Request.cpp \
			Response.cpp \
			Socket.cpp \
			Manager.cpp \
			utils.cpp

OBJS 		= ${SRCS:%.cpp=${OBJS_PATH}%.o}
DEPS		= ${SRCS:%.cpp=${DEPS_PATH}%.d}

CXX 		= c++
CXXFLAGS 	= -Wall -Wextra -Werror -std=c++98 -g

INCLUDES = -I includes -I includes/Config

all: 	${NAME}

${OBJS_PATH}%.o: ${SRCS_PATH}%.cpp ${DEPS_PATH}%.d
		@mkdir -p ${@D}
		${CXX} ${CXXFLAGS} ${INCLUDES} -MMD -MF ${DEPS_PATH}$*.d -c $< -o $@

${DEPS_PATH}%.d: ${SRCS_PATH}%.cpp
		@mkdir -p ${@D}

${NAME}: Makefile ${OBJS}
		@echo "\n----Compiling webserv----\n"
		${CXX} ${CXXFLAGS} ${OBJS} ${INCLUDES} -o ${NAME}
		@echo "\nwebserv Compiled!\n"

-include ${DEPS}

clean:
		@rm -rf ${OBJS_PATH}
		@rm -rf ${DEPS_PATH}

fclean: clean
		@rm -f ${NAME}
		@echo "\nDeleting everything!\n"

re:		fclean
		make all

.PHONY: all clean fclean re re_bonus bonus
