all: guide src
	pdfunite guide/guide.pdf src/algo.pdf src/makefile.pdf guide.pdf

guide: FORCE
	$(MAKE) -C guide

src: FORCE
	cd src; ./publish_scripts.sh

FORCE:
