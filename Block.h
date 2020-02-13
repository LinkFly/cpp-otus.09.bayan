#pragma once

#include <vector>
#include <memory>

using std::vector;

class Block {
	/*std::unique_ptr<vector<uint8_t>> bytes;*/
	uint8_t* bytes = nullptr;
	size_t _size = 0;
public:
	Block() = default;
	Block(size_t size) {
		init(size);
		/*bytes = new uint8_t[size];*/
	}
	~Block() {
		destroy();
	}
	void destroy() {
		delete[] bytes;
		bytes = nullptr;
	}
	void operator=(const Block& block) {
		init(block.size());
		// TODO Refactor using optimized code
		for (size_t i = 0; i < block.size(); i++) {
			/*(*bytes)[i] = block.get(i);*/
			bytes[i] = block.get(i);
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
		/*bytes = std::make_unique<vector<uint8_t>>(size);*/
		destroy();
		bytes = new uint8_t[size];
		_size = size;
	}
	void set(size_t idx, uint8_t value) {
		/*(*bytes)[idx] = value;*/
		bytes[idx] = value;
	}
	uint8_t get(size_t idx) const {
		/*return (*bytes)[idx];*/
		return bytes[idx];
	}
	size_t size() const { 
		/*return (*bytes).size();*/
		return _size;
	}

	void addZeros(size_t readed) {
		uint8_t* buf = ptr() + readed;
		size_t needZeros = size() - readed;
		while (needZeros--) {
			*(buf + needZeros) = 0;
		}
	}

	uint8_t* ptr() { return bytes;  }
};