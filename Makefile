##################################################
#			GLOBAL VARIABLES DEFINITION			 #
##################################################

TARGET	= scop

CFLAGS	= -Wall -Wextra --std=c++11 -g # -Werror
CC		= g++

INC 	= -I ./include				\
			-I ./libs/include		\
			-I ./ftGraphics/include	\

SRCDIR	= ./src/
SRC		= scop.cpp Model.cpp parser.cpp ShaderProgram.cpp debug.cpp

OBJDIR	= ./objs/
OBJS	= ${addprefix ${OBJDIR}, ${SRC:.cpp=.o}}

LIBS	= -L libs -lglfw3 -lGLEW -limgui -lGL -lpthread -ldl -lX11

##################################################
#				COMPILATION RULES				 #
##################################################

${OBJDIR}%.o: ${SRCDIR}%.cpp
	${CC} ${CFLAGS} -c $< ${INC} -o $@

${TARGET}: ${OBJDIR} ${OBJS} ${LIBFT}
	${CC} ${CFLAGS} ${OBJS} -o $@ ${LIBS}

${OBJDIR}:
	mkdir -p ${OBJDIR}

##################################################
#  				   USUAL RULES					 #
##################################################

all: ${TARGET}

clean:
	rm -rf ${OBJDIR}
	${MAKE} clean -C ftGraphics

fclean: clean
	rm -rf ${TARGET}
	${MAKE} fclean -C ftGraphics

re: fclean all

.PHONY:	all clean fclean re
