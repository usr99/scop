##################################################
#			GLOBAL VARIABLES DEFINITION			 #
##################################################

TARGET	= scop

CFLAGS	= -Wall -Wextra --std=c++11 -g # -Werror
CC		= g++

INC 	= -I ./include				\
			-I ./libs/include		\

SRCDIR	= ./src/
SRC		= scop.cpp Model.cpp utils.cpp

OBJDIR	= ./objs/
OBJS	= ${addprefix ${OBJDIR}, ${SRC:.cpp=.o}}

DEPS	= -L libs -lglfw3 -lGL -lpthread -ldl -lX11

##################################################
#				COMPILATION RULES				 #
##################################################

${OBJDIR}%.o: ${SRCDIR}%.cpp
	${CC} ${CFLAGS} -c $< ${INC} -o $@

${TARGET}: ${OBJDIR} ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@ ${DEPS}

${OBJDIR}:
	mkdir -p ${OBJDIR}

##################################################
#  				   USUAL RULES					 #
##################################################

all: ${TARGET}

clean:
	rm -rf ${OBJDIR}

fclean: clean
	rm -rf ${TARGET}

re: fclean all

.PHONY:	all clean fclean re
