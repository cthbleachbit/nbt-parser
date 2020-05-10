#ifndef NBTP_CONSTANTS_H
#define NBTP_CONSTANTS_H

#define IO_UNEXPECTED_EOF "Premature end-of-file"

BEGIN_NBTP_NS

/**
 * Indicates the format of related input or output buffer
 */
	enum IOFormat {
		TEXT = 0,
//		GZ = 1,
		BIN = 2,
//		YAML = 3
	};

END_NBTP_NS

#endif //NBTP_CONSTANTS_H
