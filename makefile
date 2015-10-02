CC = g++

SRC_EXT = cc
TEST_EXT = $(SRC_EXT)
OBJ_EXT = o
BIN_EXT = bin

OPTIMISATION_FLAG = #-O3
WARNING_FLAGS = -Wall -Werror
STANDARD_FLAGS = -std=c++11
STD_LIBS = \
					-lm \
					-lboost_system \
					-lboost_thread \
					-lCGAL -lgmp -lmpfr -frounding-math

CC_FLAGS = \
	$(OPTIMISATION_FLAG) \
  $(WARNING_FLAGS) \
	$(STD_LIBS) \
	$(STANDARD_FLAGS)

SRC_DIR_NAME = src
SRC_DIR = $(SRC_DIR_NAME)

SOLUTIONS = $(wildcard *.$(SRC_EXT))
SOLVERS = $(SOLUTIONS:.$(SRC_EXT)=.$(BIN_EXT))

MAIN_NAME = main
MAIN = $(MAIN_NAME).$(BIN_EXT)

all: $(SOLVERS)

%.$(BIN_EXT): %.$(OBJ_EXT)
	$(CC) -o $@ $^  $(CC_FLAGS)

%.$(OBJ_EXT): %.$(SRC_EXT)
	$(CC) -o $@ -c $< $(CC_FLAGS)

WRITEUP_SRC = writeup.tex
WRITEUP_CLS = writeup.cls
WRITEUP_OUT = writeup.pdf

publish: $(WRITEUP_OUT)

$(WRITEUP_CLS): ../$(WRITEUP_CLS)
	cp ../$(WRITEUP_CLS) .

$(WRITEUP_OUT): $(SOLUTIONS) $(WRITEUP_CLS) $(WRITEUP_SRC)
	latexmk -pdf -pdflatex="pdflatex -shell-escape -halt-on-error -enable-write18" $(WRITEUP_SRC)


DIRTY_EXT = *.$(OBJ_EXT) *.$(BIN_EXT)
DIRTY = $(DIRTY_EXT) $(BIN_NAME) $(TMP_SRC)

clean:
	rm -f $(DIRTY)


love:
	@echo not war
