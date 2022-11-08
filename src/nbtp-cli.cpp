#include "libnbtp.h"
#include <iostream>
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
	o << "Use - to specify standard input" << endl;
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

	ssize_t tagSize;
	std::shared_ptr<NBTP::Tag> tag;
	std::string input_file = argv[1];
	try {
		if (input_file == "-") {
			tag = NBTP::TagIO::parseRoot(std::cin, tagSize, NBTP::IOFormat::BIN);
		} else {
			std::ifstream in(input_file, std::ios::in | std::ios::binary);
			if (in.fail()) {
				std::cerr << "Cannot open file " << input_file << std::endl;
				exit(1);
			}
			tag = NBTP::TagIO::parseRoot(in, tagSize, NBTP::IOFormat::BIN);
		}
	} catch (NBTP::TagParseException &e) {
		std::cerr << e.what() << std::endl;
		exit(2);
	} catch (std::ios_base::failure &e) {
		std::cerr << fmt::format(IO_UNEXPECTED_EOF, e.what()) << std::endl;
	}

	std::cerr << tagSize << " bytes read" << std::endl;
	std::cout << *tag << std::endl;
	return 0;
}
