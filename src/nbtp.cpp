#include "libnbtp.h"
#include <iostream>
#include <cstring>
#include <boost/format.hpp>
#include <fstream>

#ifndef NBTP_VERSION
#define NBTP_VERSION "Unknown Version"
#endif

#ifndef NBTP_GIT_SHA1
#define NBTP_VERSION_STRING NBTP_VERSION
#else
#define NBTP_VERSION_STRING NBTP_VERSION " " NBTP_GIT_SHA1
#endif

void version(std::ostream &o) {
	o << "nbtp-cli " NBTP_VERSION_STRING << std::endl;
}

void usage(std::ostream &o) {
	using namespace std;
	o << "Usage: nbtp-cli <NBT FILE>" << endl;
	o << endl;
	o << "Example program, a single decompressed NBT viewer, with libnbtp." << endl;
	version(o);
}

int main(int argc, char **argv) {
	if (argc <= 1) {
		usage(std::cout);
		return 0;
	} else if (argc > 2) {
		std::cerr << "Too many arguments" << std::endl;
		usage(std::cout);
		return -1;
	}

	std::ifstream in(argv[1], std::ios::in | std::ios::binary);
	ssize_t tagSize;
	auto tag = NBTP::TagIO::parseRoot(in, tagSize);
	in.close();

	std::cout << tagSize << " Bytes read from file " << argv[1] << std::endl;
	std::cout << *tag << std::endl;
	return 0;
}
