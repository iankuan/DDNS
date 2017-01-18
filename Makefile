CC ?= gcc
RM ?= rm

CFLAGS = -O0 -std=gnu99 -Wall
LFLAGS =

INC_DIR = include
SRC_DIR = src
TEST_DIR = test

#TEST_SRC = test_resolver.c
LIB = 

OBJ = 
EXEC = dns_main
TEST = 

#Addition FLAGS
CFLAGS += -I $(INC_DIR)
LFLAGS +=

vpath %.c $(SRC_DIR)

ifndef TEST_SRC
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
else
TEST_SRC := $(addprefix $(TEST_DIR)/, $(TEST_SRC))
endif
TEST = $(patsubst %.c, %.out, $(TEST_SRC))

OBJ_SRC = $(wildcard $(SRC_DIR)/utility/*.c)
OBJ = $(patsubst %.c, %.o, $(OBJ_SRC)) parser.o
#src/dns_main.o

#Debug FLAGS
ifeq ($(strip $(DEBUG)), 1)
CFLAGS += -DDEBUG -g3
endif

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

#$(notdir $PATH) can left the file name without dir
%.out: %.c
	$(CC) $(CFLAGS) -DDEBUG -g3 -E $^ > tmp
	$(CC) $(CFLAGS) -DDEBUG -g3 -o $(notdir $@) $^ $(OBJ)

test: $(OBJ) $(TEST)

clean:
	$(RM) -rf $(EXEC) $(OBJ) $(notdir $(TEST))
