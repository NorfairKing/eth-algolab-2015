all: guide
	pdfunite guide/guide.pdf src/algo.pdf src/makefile.pdf guide.pdf

guide: FORCE
	$(MAKE) -C guide

FORCE:
