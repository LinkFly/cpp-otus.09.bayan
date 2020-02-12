#pragma once

#include <vector>
//#include <memory>

#include "Block.h"
#include "Config.h"

using std::vector;

class Hash {
public:
	Block hashBlock;
	Hash() = default;
	Hash(const Block& block) {
		init(block);
	}
	void init(const Block& block) {
		auto& config = Config::getInstance();
		// Not implemented (here must read config and hashes)
		hashBlock = block;
	}
	bool operator==(const Hash& hash) {
		return hashBlock == hash.hashBlock;
	}
	bool operator!=(const Hash& hash) {
		return !operator==(hash);
	}
};