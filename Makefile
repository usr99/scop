##################################################
#			GLOBAL VARIABLES DEFINITION			 #
##################################################

TARGET	= scop.out

CFLAGS	= -Wall -Wextra -Werror --std=c++11
CC		= g++

BREW_PATH = /Users/${USER}/.brew/opt/
DEPENDENCIES = glfw glew
DEPS_FULL_PATH = ${addprefix ${BREW_PATH}, ${DEPENDENCIES}}

INC = ${addsuffix /include, ${addprefix -I, ${DEPS_FULL_PATH}}} -I include
LIBS = ${addsuffix /lib, ${addprefix -L, ${DEPS_FULL_PATH}}} -lGLEW -lglfw -Llibs -limgui -framework OpenGL

SRCDIR	= ./src/
SRC		= scop.cpp Model.cpp ShaderProgram.cpp ArcballCamera.cpp LightSource.cpp ColorPalette.cpp debug.cpp \
			parser.cpp Object.cpp Material.cpp BMPimage.cpp Skybox.cpp textures.cpp

OBJDIR	= ./objs/
OBJS	= ${addprefix ${OBJDIR}, ${SRC:.cpp=.o}}

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
