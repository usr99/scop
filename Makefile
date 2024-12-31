##################################################
#			GLOBAL VARIABLES DEFINITION			 #
##################################################

TARGET	= scop

CFLAGS	= -Wall -Wextra -Werror --std=c++11
CC		= g++

INC 	= -I ./include

SRCDIR	= ./src/
SRC		= scop.cpp Model.cpp ShaderProgram.cpp ArcballCamera.cpp LightSource.cpp ColorPalette.cpp debug.cpp \
			parser.cpp Object.cpp Material.cpp BMPimage.cpp Skybox.cpp textures.cpp \
			imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp \
			imgui_impl_opengl3.cpp imgui_impl_glfw.cpp 

OBJDIR	= ./objs/
OBJS	= ${addprefix ${OBJDIR}, ${SRC:.cpp=.o}}

LIBS	= -lglfw -lGLEW -lGL

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
#  				   USUAL RULES					 #./
##################################################

test:
	@echo ${SRC}
	@echo ${OBJS}

all: ${TARGET}

clean:
	rm -rf ${OBJDIR}

fclean: clean
	rm -rf ${TARGET}
	rm -f imgui.ini

re: fclean all

.PHONY:	all clean fclean re
