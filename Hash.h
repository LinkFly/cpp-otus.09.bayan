#pragma once

#include <vector>
//#include <memory>
#include <cstring>

#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>

#include "Block.h"
#include "Config.h"

using std::vector;

class Hash {
	//// Utils
	void uint8ArrayToHash(const uint8_t* arBytes, size_t arSize) {
		hashBlock.init(arSize);
		memcpy(hashBlock.ptr(), arBytes, arSize);
	}
	//// end Utils

	void handlingCRC32(Block& block) {
		boost::crc_32_type  result;
		const uint8_t bytes[] = { 0x20, 0x21, 0x22, 0x23, 0x24 };
		result.process_bytes(block.ptr(), block.size());
		uint32_t intRes = result.checksum();
		uint8ArrayToHash(reinterpret_cast<uint8_t*>(&intRes), sizeof(intRes));
	}

	void handlingMD5(Block& block) {
		using boost::uuids::detail::md5;
		md5 hash;
		md5::digest_type digest;
		hash.process_bytes(block.ptr(), block.size());
		hash.get_digest(digest);
		uint8ArrayToHash(reinterpret_cast<uint8_t*>(&digest), sizeof(digest));
	}



public:
	Block hashBlock;
	Hash() = default;
	// TODO! To mind: how use const Block&
	Hash(Block& block) {
		
		init(block);
	}
	void init(Block& block) {
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
			handlingCRC32(block);
			break;
		case Config::SupportedHashTypes::MD5:
			handlingMD5(block);
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