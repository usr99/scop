##################################################
#			GLOBAL VARIABLES DEFINITION			 #
##################################################

TARGET	= scop.out

CFLAGS	= -Wall -Wextra --std=c++11 -g # -Werror
CC		= g++

INC 	= -I ./include				\
			-I ./libs/include		\
			-I ./ftGraphics/include	\

SRCDIR	= ./src/
SRC		= scop.cpp Model.cpp ShaderProgram.cpp ArcballCamera.cpp LightSource.cpp ColorPalette.cpp debug.cpp \
			parser.cpp Object.cpp Material.cpp BMPimage.cpp Skybox.cpp textures.cpp


OBJDIR	= ./objs/
OBJS	= ${addprefix ${OBJDIR}, ${SRC:.cpp=.o}}

LIBS	= -L libs -lglfw3 -lGLEW -limgui -lGL -lpthread -ldl -lX11

##################################################
#				COMPILATION RULES				 #
##################################################

${OBJDIR}%.o: ${SRCDIR}%.cpp ./include/%.hpp
	${CC} ${CFLAGS} -c $< ${INC} -o $@

${TARGET}: ${OBJDIR} ${OBJS} ${LIBFT}
	${CC} ${CFLAGS} ${OBJS} -o $@ ${LIBS}

${OBJDIR}:
	mkdir -p ${OBJDIR}

##################################################
#  				   USUAL RULES					 #./
##################################################

all: ${TARGET}

clean:
	rm -rf ${OBJDIR}

fclean: clean
	rm -rf ${TARGET}

re: fclean all

.PHONY:	all clean fclean re
