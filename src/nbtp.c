#include "nbtp.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define FAIL(msg) fprintf(stderr, "%s", msg)

int termWidth() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_col;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		FAIL("Usage: nbtp-cli <input-nbt>\n");
		FAIL("        Example application. Displays information about uncompressed nbt files\n");
		fprintf(stderr, "Library version: %s %s\n", VERSION, GIT_SHA1);
		exit(1);
	}
	int fd = open(argv[1], 'r');
	if (fd == -1) {
		FAIL("Cannot open file");
		exit(1);
	}
	struct stat st;
	stat(argv[1], &st);
	int size = st.st_size;

	void *bin = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (bin == MAP_FAILED) {
		FAIL("Cannot map file to memory region");
		exit(127);
	}
	tag *nbt = parseFile(bin, size);
	int expect = nbt_binary_size(nbt);
	printf("Incoming bytes: %i, Computed size: %i.\n", size, expect);
	printTag(nbt, stdout, 0, termWidth());
	munmap(bin, size);
	close(fd);

	freeTag(nbt);
	return 0;
}
