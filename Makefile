CC ?= gcc
RM ?= rm

CFLAGS = -std=gnu99 -Wall
LFLAGS =

INC_DIR = include
SRC_DIR = src
TEST_DIR = test

TEST_SRC =
LIB = 

OBJ =
EXEC =
TEST = 

#Addition FLAGS
CFLAGS += -I $(INC_DIR)
LFLAGS +=

vpath %.c $(SRC_DIR)
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TEST = $(patsubst %.c, %.out, $(TEST_SRC))
OBJ = $(patsubst %.c, %.o, $(SRC))


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

#$(notdir $PATH) can left the file name without dir
%.out: %.c
	$(CC) $(CFLAGS) -o $(notdir $@) $^

test: $(TEST)

clean:
	$(RM) -rf $(EXEC) $(OBJ) $(notdir $(TEST))
