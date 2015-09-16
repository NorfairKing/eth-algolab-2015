LANG = c++

CC = g++

SRC_EXT = cc
TEST_EXT = $(SRC_EXT)
OBJ_EXT = o
BIN_EXT = bin

OPTIMISATION_FLAG = -O3
WARNING_FLAGS = -Wall -Werror
STANDARD_FLAGS = -std=c++11
STD_LIBS = -lm

CC_FLAGS = \
	$(OPTIMISATION_FLAG) \
  $(WARNING_FLAGS) \
	$(STD_LIBS) \
	$(STANDARD_FLAGS)

SRC_DIR_NAME = src
SRC_DIR = $(SRC_DIR_NAME)

SRCS = $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
OBJS = $(SRCS:.$(SRC_EXT)=.$(OBJ_EXT))

MAIN_NAME = main
MAIN = $(MAIN_NAME).$(BIN_EXT)

all: $(MAIN) 

%.bin : $(OBJS)
	$(CC) -o $@ $^  $(CC_FLAGS)

%.$(OBJ_EXT): %.$(SRC_EXT)
	$(CC) -o $@ -c $< $(CC_FLAGS)


DIRTY_EXT = *.$(OBJ_EXT) *.$(BIN_EXT)
DIRTY = $(DIRTY_EXT) $(BIN_NAME) $(TMP_SRC)

clean:
	rm -f $(DIRTY)


love:
	@echo not war
