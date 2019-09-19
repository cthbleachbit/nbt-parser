#ifndef NBTP_EXCEPTION_H
#define NBTP_EXCEPTION_H

#include <bits/exception.h>
#include <cstring>

BEGIN_NBTP_NS

class InvalidTagException : public std::exception {
private:
	size_t offset;
public:
	explicit InvalidTagException(size_t _offset) : offset(_offset) {};
	virtual const char* what() const noexcept;
	virtual size_t getOffset() {
		return this->offset;
	}
};

class NotRootTagException : public std::exception {
public:
	virtual const char* what() const noexcept;
};

END_NBTP_NS

#endif //NBTP_EXCEPTION_H
