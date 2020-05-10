#ifndef NBTP_CONSTANTS_H
#define NBTP_CONSTANTS_H

#define IO_UNEXPECTED_EOF   "Premature end-of-file"
#define CONTENT_LEN_NEG     "Content length is negative"
#define LIST_INVALID_TYPE   "Invalid content type in list"
#define LIST_CHANGE_TYPE    "Changing type on a non-empty list is not allowed"
#define LIST_ADD_UNMATCH    "The tag to add has a different type than others"
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
