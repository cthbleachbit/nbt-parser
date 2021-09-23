#ifndef NBTP_CONSTANTS_H
#define NBTP_CONSTANTS_H

// Error messages
// Invalid input data / parameter
#define CONTENT_LEN_NEG     "Content length is negative"
#define INVALID_TYPE        "Invalid content type"
#define GENERIC_METHOD      "Generic tag method"
#define IO_UNEXPECTED_EOF   "Premature end-of-file"
#define PARSE_PRETTY        "Parsing pretty input isn't supported yet"
#define PARSE_UNKNOWN_FMT   "Unknown IOFormat"
#define SUBTAG_NOT_FOUND    "Tag not in compound"

// Disallowed operations
#define LIST_CHANGE_TYPE    "Changing type on a non-empty list is not allowed"
#define LIST_ADD_UNMATCH    "Inserting a {} tag into a list with different type of contents {}"

// Compatibility issues
#define LIST_END_NZ_LEN     "The list is marked with content type END but has non-zero length"
#define LIST_TOO_LONG       "The list is too long for NBT format"
#define STRING_TOO_LONG     "String field is too long"

// Warning messages
#define ROOT_NOT_COMPOUND   "Root tag is not a compound"
#define ROOT_HAS_NAME       "Root tag has non-empty name string"

#define REPR_SINGLE_VALUED    "({}) {}" // Type + value

namespace NBTP {

	/**
	 * Indicates the format of related input or output buffer
	 */
	enum IOFormat {
		PRETTY_PRINT = 0,
//		SNBT = 1,
		BIN = 2,
//		YAML = 3
	};

}

#endif //NBTP_CONSTANTS_H
