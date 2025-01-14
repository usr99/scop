##################################################
#			GLOBAL VARIABLES DEFINITION			 #
##################################################

TARGET	= scop

CFLAGS	= -Wall -Wextra -Werror --std=c++11
CC		= g++

IMGUIDIR= imgui
IMGUI   = ${IMGUIDIR}/libimgui.a

INC 	= -I ./include -I ./${IMGUIDIR}/include

SRCDIR	= ./src/
SRC		= scop.cpp Model.cpp ShaderProgram.cpp ArcballCamera.cpp LightSource.cpp ColorPalette.cpp debug.cpp \
			parser.cpp Object.cpp Material.cpp BMPimage.cpp Skybox.cpp textures.cpp

OBJDIR	= ./objs/
OBJS	= ${addprefix ${OBJDIR}, ${SRC:.cpp=.o}}

LIBS	= -lglfw -lGLEW -lGL -L ${IMGUIDIR} -limgui

##################################################
#				COMPILATION RULES				 #
##################################################

${OBJDIR}%.o: ${SRCDIR}%.cpp
	${CC} ${CFLAGS} -c $< ${INC} -o $@

${TARGET}: ${OBJDIR} ${OBJS} ${IMGUI}
	${CC} ${CFLAGS} ${OBJS} -o $@ ${LIBS}

${IMGUI}:
	${MAKE} -C ${IMGUIDIR}

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
	rm -f imgui.ini
	${MAKE} -C ${IMGUIDIR} fclean

re: fclean all

.PHONY:	all clean fclean re
