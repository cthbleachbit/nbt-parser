#include "tag.h"
#include "parser.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	if (argc != 2) { return 1; }
	int fd = open(argv[1], 'r');
	struct stat st;
	stat(argv[1], &st);
	int size = st.st_size;

	void *bin = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
	tag *nbt = parseFile(bin, size);
	printTag(nbt, stdout, 0);
	munmap(bin, size);
	close(fd);
	return 0;
}
