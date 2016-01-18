
all: guide
	cp guide/guide.pdf .

guide: FORCE
	$(MAKE) -C guide

FORCE:
