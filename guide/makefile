GUIDE_SRC = guide.tex
GUIDE_CLS = guide.cls
GUIDE_OUT = guide.pdf

all: publish

publish: $(GUIDE_OUT)

TEXS = $(wildcard *.tex)
EXAMPLES = $(wildcard examples/*.cc)

$(GUIDE_OUT): $(GUIDE_SRC) $(TEXS) $(EXAMPLES)
	latexmk -pdf -pdflatex="pdflatex -shell-escape -halt-on-error -enable-write18" $(GUIDE_SRC)
	latexmk -c

love:
	@echo not war
