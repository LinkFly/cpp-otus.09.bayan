#pragma once

#include <vector>
#include <memory>

using std::vector;

class Block {
	std::unique_ptr<vector<uint8_t>> bytes;

public:
	Block() = default;
	Block(size_t size) {
		init(size);
	}
	void operator=(const Block& block) {
		init(block.size());
		// TODO Refactor using optimized code
		for (size_t i = 0; i < block.size(); i++) {
			(*bytes)[i] = block.get(i);
		}
	}
	bool operator==(const Block& block) {
		auto curSize = block.size();
		if (size() != curSize) {
			return false;
		}
		// TODO Refactor: need optimized code
		for (size_t i = 0; i < curSize; i++) {
			if (get(i) != block.get(i)) {
				return false;
			}
		}
		return true;
	}
	void init(size_t size) {
		bytes = std::make_unique<vector<uint8_t>>(size);
	}
	void set(size_t idx, uint8_t value) {
		(*bytes)[idx] = value;
	}
	uint8_t get(size_t idx) const {
		return (*bytes)[idx];
	}
	size_t size() const { 
		return (*bytes).size();
	}
};