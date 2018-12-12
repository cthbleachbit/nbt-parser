#include "nbtp.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define FAIL(msg) fprintf(stderr, "%s\n", msg)

int termWidth() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_col;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		FAIL("Usage: nbtp-cli <input-nbt>");
		FAIL("        Simple example application. Displays information about nbt files");
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
	printTag(nbt, stdout, 0, termWidth());
	munmap(bin, size);
	close(fd);

	freeTag(nbt);
	return 0;
}
