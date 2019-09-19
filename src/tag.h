#ifndef NBTP_TAG_H
#define NBTP_TAG_H

#include "io.h"

#include <cstdint>
#include <vector>
#include <string>
#include <map>

BEGIN_NBTP_NS

enum TagType {
	INVALID = -1,
	END = 0,
	BYTE = 1,
	SHORT = 2,
	INT = 3,
	LONG = 4,
	FLOAT = 5,
	DOUBLE = 6,
	BYTES = 7,
	STRING = 8,
	LIST = 9,
	COMPOUND = 10,
	INTS = 11,
	LONGS = 12
};

/**
 * A Tag is defined as either a single value or a container of key-tag pairs
 *
 * A tag by itself carries no name. The name attribute of a tag is contained in its patent tag as the key.
 * A compound tag is a key-tag hash map.
 * A list tag is a vector of tags
 */
class Tag {
public:
	virtual void output(IOFormat);
	virtual int8_t typeCode() { return TagType::INVALID ; };
	void output()
	{
		output(IOFormat::TEXT);
	}
};

/*
 * These tags has a fixed length
 */
class ByteTag : public Tag {
public:
	int8_t value;
	int8_t typeCode() { return TagType::BYTE; };
	ByteTag(int8_t _value) : value(_value) {};
};

class ShortTag : public Tag {
public:
	int16_t value;
	int8_t typeCode() { return TagType::SHORT; };
	ShortTag(int16_t _value) : value(_value) {};
};

class IntTag : public Tag {
public:
	int32_t value;
	int8_t typeCode() { return TagType::INT; };
	IntTag(int32_t _value) : value(_value) {};
};

class LongTag : public Tag {
public:
	int64_t value;
	int8_t typeCode() { return TagType::LONG; };
	LongTag(int64_t _value) : value(_value) {};
};

class FloatTag : public Tag {
public:
	float value;
	int8_t typeCode() { return TagType::FLOAT; };
	FloatTag(float _value) : value(_value) {};
};

class DoubleTag : public Tag {
public:
	double value;
	int8_t typeCode() { return TagType::DOUBLE; };
	DoubleTag(double _value) : value(_value) {};
};

// Following tags may contain dynamic payload sizes

/*
 * List and compound tags share largely the same data structure. The only difference is that
 * compound tags store full tags, but lists store payloads in the same type only. ListTag is a
 * "special" case where __names__ of subtags are not important and simply ignored.
 */
class CompoundTag : public Tag {
public:
	bool isRoot = false;
protected:
	std::map<std::string, Tag> container;
public:
	CompoundTag(bool isRoot) : isRoot(isRoot) {};
	int8_t typeCode() { return TagType::COMPOUND; };
	size_t size() {
		return container.size();
	}
	void add(const std::string &name, Tag & tag) {
		container[name] = tag;
	}
	Tag& get(const std::string &name) {
		return container[name];
	};
};

class ListTag : public CompoundTag {
protected:
	std::vector<Tag> container;
public:
	int8_t typeCode() { return TagType::LIST; };
	void add(Tag& tag) {
		container.push_back(tag);
	}
	void add(const std::string &name, Tag& tag) {
		add(tag);
	}
};

/*
 * The rest of variable length tags are constructured as special cases of ListTag
 */
class ByteArrayTag : public ListTag {
public:
	int8_t typeCode() { return TagType::BYTES; };
};

class StringTag : public ByteArrayTag {
public:
	// Note that String in Minecraft is essentially the same as a byte array
	int8_t typeCode() { return TagType::STRING; };
};

class IntArrayTag : public ListTag {
public:
	int8_t typeCode() { return TagType::INTS; };
};

class LongArray : public ListTag {
public:
	int8_t typeCode() { return TagType::LONGS; };
};

END_NBTP_NS

#endif