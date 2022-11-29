NAME	= webserv

SRCS_PATH = srcs/
OBJS_PATH = objs/
DEPS_PATH = deps/

SRCS 	=	main.cpp \
			Server.cpp \
			Location.cpp \
			Config.cpp \
			Request.cpp \
			Response.cpp \
			Socket.cpp \
			Manager.cpp \
			utils.cpp

OBJS 		= $(addprefix $(OBJS_PATH), ${SRCS:.cpp=.o})
DEPS		= $(OBJS:.o=.d)

CXX 		= c++
CXXFLAGS 	= -Wall -Wextra -Werror -std=c++98 -g

INCLUDES = includes

all: 	${NAME}

${OBJS_PATH}%.o: ${SRCS_PATH}%.cpp ${DEPS_PATH}%.d
		@mkdir -p ${OBJS_PATH}
		${CXX} ${CXXFLAGS} -I${INCLUDES} -MMD -MP -o $@ -c $<

${DEPS_PATH}%.d: ${SRCS_PATH}%.cpp
		@mkdir -p ${DEPS_PATH}

${NAME}: ${OBJS}
		@echo "\n----Compiling webserv----\n"
		${CXX} ${CXXFLAGS} ${OBJS} -o ${NAME}
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

.PHONY: all clean fclean re
