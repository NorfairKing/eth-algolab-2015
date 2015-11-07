
all: guide writeups
	pdfunite guide/guide.pdf src/algolab-writeups-2015.pdf guide.pdf

writeups: FORCE
	cd src && ./publish_all.sh

guide: FORCE
	$(MAKE) -C guide

FORCE:
