//
// Created by cth451 on 2020/05/10.
//

#ifndef NBTP_MEMSTREAM_H
#define NBTP_MEMSTREAM_H


#include <istream>

class membuf : public std::basic_streambuf<char> {
public:
	membuf(const uint8_t *p, size_t l) {
		setg((char *) p, (char *) p, (char *) p + l);
	}
};

class memstream : public std::iostream {
public:
	memstream(const uint8_t *p, size_t l) :
			std::iostream(&_buffer),
			_buffer(p, l) {
		rdbuf(&_buffer);
	}

private:
	membuf _buffer;
};

#endif //NBTP_MEMSTREAM_H
