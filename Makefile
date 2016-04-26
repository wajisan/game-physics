SRC=		main.cpp \
		Core.cpp

NAME=		cubeWorld

OBJ_SRC= 	$(SRC:.cpp=.o)

all:		$(NAME);

$(NAME):	$(OBJ_SRC)
		g++ $(OBJ_SRC) -o $(NAME) -lSDL -lGL -lBox2D

clean:		
	rm -f $(OBJ_SRC)

fclean:		clean
	rm -f $(NAME)

make re:	fclean all
