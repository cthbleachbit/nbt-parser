#include "exception.h"

BEGIN_NBTP_NS

const char* InvalidTagException::what() const noexcept {
	return "Attempt to process invalid tag binary";
}

const char* NotRootTagException::what() const noexcept {
	return "Root Tag is not marked as root";
}

END_NBTP_NS