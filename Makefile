CC ?= gcc
RM ?= rm

CFLAGS = -std=gnu99 -Wall
LFLAGS =

INC = include
SRC = src
TEST_SRC = test
LIB = 

OBJ =
EXEC =
TEST =

#Addition FLAGS
CFLAGS += -I $(INC)
LFLAGS +=

vpath %.c $(SRC)
vpath %.c $(TEST_SRC)

#Debug FLAGS
ifeq ($(strip $(DEBUG)), 1)
	CFLAGS += -DDEBUG -g3
endif

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^

test: $(TEST)

clean:
	$(RM) -rf $(EXEC) $(OBJ)
