/* Tag syntax tree nodes
 *
 */

#include "io.h"

#include <cstdint>

namespace nbtp {

class Tag {
public:
	std::string name;

	virtual void output(IOFormat fmt);
	virtual void output()
	{
		output(IOFormat::TEXT);
	}
}

/*
 * These tags has a fixed length
 */
class ByteTag : public Tag {
public:
	int8_t value;
}

class ShortTag : public Tag {
public:
	int16_t value;
}

class IntTag : public Tag {
public:
	int32_t value;
}

class LongTag : public Tag {
pubic:
	int64_t value;
}

class FloatTag : public Tag {
public:
	float value;
}

class DoubleTag : public Tag {
public:
	double value;
}

// These tags may contain dynamic payload sizes

/*
 * A prototype DynamicTag, serving as the basis for all other tags.
 */
class DynamicTag : public Tag {
public:
	std::vector<Tag> payload;
}

/*
 * List and compound tags share largely the same data structure. The only difference is that
 * compound tags store full tags, but lists store payloads only. Here I'm using lists as a
 * "special" situation where __names__ of subtags are not important.
 */
class CompoundTag : public DynamicTag {
}

class ListTag : public Compound {
}

/*
 * The rest of variable length tags are constructured as special cases of ListTag
 */
class ByteArrayTag : public ListTag {
}

class StringTag : public ByteArrayTag {
	// Note that String in Minecraft is essentially the same as a byte array
}

class IntArrayTag : public ListTag {
}

class LongArray : public ListTag {
}
