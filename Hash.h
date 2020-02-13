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
		if (Config::curHashType == Config::SupportedHashTypes::Debug) {
			hashBlock = block;
		}
		else {
			// TODO!!!!! Not implemented (here must read config and hashes)
		}
	}
	bool operator==(const Hash& hash) {
		return hashBlock == hash.hashBlock;
	}
	bool operator!=(const Hash& hash) {
		return !operator==(hash);
	}
};