##################################################
#			GLOBAL VARIABLES DEFINITION			 #
##################################################

TARGET	= scop

CFLAGS	= -Wall -Wextra --std=c++11 -g # -Werror
CC		= g++

INC 	= -I ./include				\
			-I ./deps				\
			-I ./deps/GLFW/include	\
			-I ./deps/GLEW/include	

SRCDIR	= ./src/
SRC		= scop.cpp Model.cpp utils.cpp

OBJDIR	= ./objs/
OBJS	= ${addprefix ${OBJDIR}, ${SRC:.cpp=.o}}

################## DEPENDENDIES ##################
GLFW_B	= deps/GLFW/build
GLFW	= ${GLFW_B}/src/libglfw3.a

GLEW_B	= deps/GLEW
GLEW	= ${GLEW_B}/lib/libGLEW.a

IMGUI_B	= deps/imgui/
IMGUI	= ${IMGUI_B}/libimgui.a

DEPS	= -L${GLFW_B}/src -lglfw3 -L${IMGUI_B} -limgui -L${GLEW_B}/lib -lGLEW -lGL -lpthread -ldl -lX11

##################################################
#				COMPILATION RULES				 #
##################################################

${OBJDIR}%.o: ${SRCDIR}%.cpp
	${CC} ${CFLAGS} -c $< ${INC} -o $@

${TARGET}: ${OBJDIR} ${OBJS} ${GLFW} ${GLEW} ${IMGUI}
	${CC} ${CFLAGS} ${OBJS} -o $@ ${DEPS}

${OBJDIR}:
	mkdir -p ${OBJDIR}

############ DEPENDENDIES COMPILATION ############

${GLFW}: ${GLFW_B}
	${MAKE} -C ${GLFW_B}

${GLFW_B}:
	mkdir ${GLFW_B}
	cd ${GLFW_B} && cmake ..

${GLEW}:
	${MAKE} glew.lib -C ${GLEW_B}
# Delete dynamic library to avoid conflict with the static one
	rm -rf ${GLEW_B}/lib/libGLEW.so

${IMGUI}:
	${MAKE} -C ${IMGUI_B}

##################################################
#  				   USUAL RULES					 #
##################################################

all: ${TARGET}

clean:
	rm -rf ${OBJDIR}
	${MAKE} clean -C ${GLFW_B}
	${MAKE} clean -C ${GLEW_B}
	${MAKE} clean -C ${IMGUI_B}	

fclean: clean
	rm -rf ${TARGET}
	rm -rf ${GLFW_B}
	${MAKE} fclean -C ${IMGUI_B}

re: fclean all

.PHONY:	all clean fclean re