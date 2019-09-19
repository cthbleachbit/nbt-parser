#ifndef NBTP_PARSER_H
#define NBTP_PARSER_H

#include <iostream>

#include "tag.h"

BEGIN_NBTP_NS

/**
 * \brief This class is a state machine to parse uncompressed binary NBT to a Tag structure.
 */
class BinaryParser {
protected:
	/**
	 * The vector that will store the uncompressed binary NBT
	 */
	std::vector<char> input;
private:
	/**
	 * Number of bytes already read from the vector
	 */
	size_t offset;
	/**
	 * Status code for the parser, non-zero values indicates error
 	*/
	int status = 0;
public:
	/**
	 * Indicates whether this parser is ready to run the parse() function.
	 * @return false if the input vector is empty or has reached the end, or error has occurred during parsing.
	 */
	bool ready_to_parse();
	/**
	 * Run parse on input vector
	 * @param root a pre-constructed root tag, will be populated with parsed tags if the parsing succeeds
	 * @return 0 on success,1 on error
	 */
	int parse(CompoundTag* root);
};

END_NBTP_NS

#endif //NBTP_PARSER_H
