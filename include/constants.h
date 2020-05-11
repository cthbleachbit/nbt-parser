#ifndef NBTP_CONSTANTS_H
#define NBTP_CONSTANTS_H

// Error messages
#define IO_UNEXPECTED_EOF   "Premature end-of-file"
#define CONTENT_LEN_NEG     "Content length is negative"
#define INVALID_TYPE        "Invalid content type"
#define GENERIC_METHOD      "Generic tag method"
#define LIST_CHANGE_TYPE    "Changing type on a non-empty list is not allowed"
#define LIST_ADD_UNMATCH    "The tag to add has a different type than others"
#define LIST_END_NZ_LEN     "The list is marked with content type END but has non-zero length"
#define LIST_TOO_LONG       "The list is too long for NBT format"
#define STRING_TOO_LONG     "String field is too long"

// Warning messages
#define ROOT_NOT_COMPOUND   "Root tag is not a compound"
#define ROOT_HAS_NAME       "Root tag has non-empty name string"

BEGIN_NBTP_NS

	/**
	 * Indicates the format of related input or output buffer
	 */
	enum IOFormat {
		PRETTY_PRINT = 0,
//		SNBT = 1,
		BIN = 2,
//		YAML = 3
	};

END_NBTP_NS

#endif //NBTP_CONSTANTS_H
