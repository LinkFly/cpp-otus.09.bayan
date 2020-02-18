#pragma once

#include <vector>
#include <memory>

using std::vector;

class Block : public vector<uint8_t> {
public:
	Block() = default;
	Block(size_t size) : vector<uint8_t>(size) {}
	void addZeros(size_t readed) {
		uint8_t* buf = data() + readed;
		size_t needZeros = size() - readed;
		memset(buf, 0, needZeros);
	}
};
