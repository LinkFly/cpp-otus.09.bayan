#pragma once

#include <vector>
//#include <memory>
#include <cstring>

#include <boost/crc.hpp>

#include "Block.h"
#include "Config.h"

using std::vector;

class Hash {
	uint32_t evalCRC32(const Block& block) {
		boost::crc_32_type  result;
		const uint8_t bytes[] = { 0x20, 0x21, 0x22, 0x23, 0x24 };
		result.process_bytes(bytes, sizeof(bytes));
		return result.checksum();
	}

	void handlingCRC32(const Block& block) {
		uint32_t intRes = evalCRC32(block);
		hashBlock.init(sizeof(intRes));
		memcpy(hashBlock.ptr(), reinterpret_cast<uint8_t*>(&intRes), sizeof(intRes));
	}
public:
	Block hashBlock;
	Hash() = default;
	Hash(const Block& block) {
		init(block);
	}
	void init(const Block& block) {
		//if (Config::curHashType == ) {
		//	hashBlock = block;
		//}
		//else {
		//	// TODO!!!!! Not implemented (here must read config and hashes)
		//}
		switch (Config::curHashType) {
		case Config::SupportedHashTypes::Debug:
			hashBlock = block;
			break;
		case Config::SupportedHashTypes::CRC32:
			evalCRC32(block);
			break;
		}
	}
	bool operator==(const Hash& hash) {
		return hashBlock == hash.hashBlock;
	}
	bool operator!=(const Hash& hash) {
		return !operator==(hash);
	}
};