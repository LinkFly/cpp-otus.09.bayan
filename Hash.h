#pragma once

#include <cstring>

#include <boost/crc.hpp>
//#include <boost/uuid/detail/md5.hpp>
//#include <boost/uuid/detail/sha1.hpp>

#include "Block.h"

class Hash {
	//// Utils
	void uint8ArrayToHash(const uint8_t* arBytes, size_t arSize) {
		hashBlock.resize(arSize);
		memcpy(hashBlock.data(), arBytes, arSize);
	}
	//// end Utils

	void crc32(const Block& block) {
		boost::crc_32_type  result;
		result.process_bytes(block.data(), block.size());
		uint32_t intRes = result.checksum();
		uint8ArrayToHash(reinterpret_cast<uint8_t*>(&intRes), sizeof(intRes));
	}

	//void md5(const Block& block) {
	//	using boost::uuids::detail::md5;
	//	md5 hash;
	//	md5::digest_type digest;
	//	hash.process_bytes(block.data(), block.size());
	//	hash.get_digest(digest);
	//	uint8ArrayToHash(reinterpret_cast<uint8_t*>(&digest), sizeof(digest));
	//}

	//void sha1(const Block& block) {
	//	using boost::uuids::detail::sha1;
	//	sha1 hash;
	//	sha1::digest_type digest;
	//	hash.process_bytes(block.data(), block.size());
	//	hash.get_digest(digest);
	//	uint8ArrayToHash(reinterpret_cast<uint8_t*>(&digest), sizeof(digest));
	//}

public:
	Block hashBlock;
	Hash() = default;
	Hash(const Block& block, SupportedHashType hashType) {
		init(block, hashType);
	}
	void init(const Block& block, SupportedHashType hashType) {
		switch (hashType) {
		case SupportedHashType::Debug:
			hashBlock = block;
			break;
		case SupportedHashType::CRC32:
			crc32(block);
			break;
		//case SupportedHashType::MD5:
		//	md5(block);
		//	break;
		//case SupportedHashType::SHA1:
		//	sha1(block);
		//	break;
		}
	}
	bool operator==(const Hash& hash) {
		return hashBlock == hash.hashBlock;
	}
	bool operator!=(const Hash& hash) {
		return !operator==(hash);
	}
};