COMPILER = gcc
FILESYSTEM_FILES = sffs.c

build: $(FILESYSTEM_FILES)
	$(COMPILER) $(FILESYSTEM_FILES) -o sffs `pkg-config fuse --cflags --libs`
	echo 'To Mount: ./sffs -f [mount point]'

clean:
	rm sffs