NAME = ircserv
DIR_OBJS = .objs
DIR_DEPS = .deps
SRC = \
	src/main.cpp \
	\
	src/class/cpp/server.cpp \
	src/class/cpp/client.cpp \
	src/class/cpp/epoll.cpp \
	src/class/cpp/channel.cpp \
	src/class/cpp/numeric_rpl.cpp \
	\
	src/command/pass.cpp \
	src/command/join.cpp \
	src/command/nick.cpp \
	src/command/user.cpp \
	src/command/mode.cpp \
	\
	src/utils.cpp

INCS = \
	-Isrc/class/hpp/

OBJS=$(SRC:%.cpp=$(DIR_OBJS)/%.o)
DEPS=$(SRC:%.cpp=$(DIR_DEPS)/%.d)

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g3 -std=c++98

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(INCS) -o $@

$(DIR_OBJS)/%.o: %.cpp Makefile
	mkdir -p $(dir $@) $(DIR_DEPS)/$(dir $*)
	$(CXX) $(CXXFLAGS) $(INCS) -MMD -MP -MF $(DIR_DEPS)/$*.d -c -o $@ $<

clean:
	rm -rf $(DIR_OBJS)
	rm -rf $(DIR_DEPS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)
