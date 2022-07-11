##################################################
#			GLOBAL VARIABLES DEFINITION			 #
##################################################

TARGET	= libftGraphics.a

CFLAGS	= -Wall -Wextra --std=c++11 -g # -Werror
CC		= g++

INC 	= -I ./include				\
			-I ../libs/include		\

SRCDIR	= ./src/
SRC		= Shader.cpp VertexBufferLayout.cpp VertexArray.cpp debug.cpp

OBJDIR	= ./objs/
OBJS	= ${addprefix ${OBJDIR}, ${SRC:.cpp=.o}}

##################################################
#				COMPILATION RULES				 #
##################################################

${OBJDIR}%.o: ${SRCDIR}%.cpp
	${CC} ${CFLAGS} -c $< ${INC} -o $@

${TARGET}: ${OBJDIR} ${OBJS}
	ar -rcs $@ ${OBJS}

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
