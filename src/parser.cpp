#include "parser.h"
#include "errmsg.h"

BEGIN_NBTP_NS

bool BinaryParser::ready_to_parse() {
	return (this->input.size() >= 0)
			|| (this->offset <= this->input.size())
			|| (this->state != ParserState::OK);
}

int BinaryParser::recursive_parse(bool isRoot) {
	// Read in type byte
	char type_byte = this->input[offset];
	offset++;

	if (isRoot) {
		if (type_byte != TagType::COMPOUND) {
			this->state = ParserState::INVALID_ROOT;
		}
	}
	return 0;
}

int BinaryParser::parse(NBTP::CompoundTag *root) {
	// Pre-checking if we can write to this root tag and if we can start working
	if ((root == NULL) || (!root->isRoot)) { this->state = ParserState::INVALID_ROOT; return 1; }
	if (!this->ready_to_parse()) { return 1; };

	return this->recursive_parse(true);
}

END_NBTP_NS