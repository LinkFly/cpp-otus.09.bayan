#pragma once

#include <string>
#include <map>

#include "share-types.h"

using std::string;
using std::map;

class BayanConfig {
	static inline std::map<string, SupportedHashTypes> hashTypeMap = {
	{"crc32", SupportedHashTypes::CRC32},
	{"md5", SupportedHashTypes::MD5},
	{"sha1", SupportedHashTypes::SHA1}
	};
public:
	BayanConfig() = default;
	BayanConfig(size_t blockSize, SupportedHashTypes curHashType) {

	}
	static bool toSupportedHashType(const string& sHashType, SupportedHashTypes& hashType) {
		auto res = hashTypeMap.find(sHashType);
		if (res == hashTypeMap.end()) {
			return false;
		}
		hashType = res->second;
		return true;
	}

	bool isDisableCache = true;
	uint8_t defaultBlockSize = 5;
	SupportedHashTypes curHashType = SupportedHashTypes::CRC32;
	size_t blockSize = defaultBlockSize;
};