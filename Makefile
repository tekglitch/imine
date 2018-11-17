CC = g++

CFLAGS = -std=c++17 -Wextra -Wnon-virtual-dtor -pedantic

INCS = -I./inc  -I./inc/b64

LFLAGS = -L./lib

LIBS = -lssl -lcrypto -lstdc++fs

SRCS = ./src/main.cpp ./src/imine.cpp ./src/epoll.cpp ./src/sockets.cpp


OBJS = $(SRCS:%.cpp=%.o)
OBJS := $(OBJS:./src/%=./obj/%)

EXEC = imine


all:
	@echo $(OBJS)




$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(INCS) -o $(EXEC) $(OBJS) $(LFLAGS) $(LIBS)

./obj/%.o: ./src/%.cpp
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@



clean:
	$(RM) ./obj/*.o $(EXEC)

depend: $(SRCS)
	makedepend $(INCS) $^


