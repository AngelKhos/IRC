NAME = IRC
SRC = \
	src/main.cpp \
	\
	src/class/cpp/server.cpp

DEPS = \
	src/class/hpp/server.hpp
CXX = c++ -Wall -Wextra -Werror -std=c++98

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): Makefile $(DEPS) $(SRC)
	$(CXX) $(SRC) -o $(NAME)

clean:
	rm -rf $(NAME)

fclean: clean

re: fclean all
