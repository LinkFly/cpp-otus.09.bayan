#pragma once

#include <string>
#include <map>

using std::string;
using std::map;

class Config {
	static inline std::map<string, SupportedHashType> hashTypeMap = {
	{"crc32", SupportedHashType::CRC32},
	{"md5", SupportedHashType::MD5},
	{"sha1", SupportedHashType::SHA1}
	};
public:
	Config() = default;
	Config(size_t blockSize, SupportedHashType curHashType) {

	}
	static bool toSupportedHashType(const string& sHashType, SupportedHashType& hashType) {
		auto res = hashTypeMap.find(sHashType);
		if (res == hashTypeMap.end()) {
			return false;
		}
		hashType = res->second;
		return true;
	}

	bool isDisableCache = true;
	uint8_t defaultBlockSize = 5;
	SupportedHashType curHashType = SupportedHashType::CRC32;
	size_t blockSize = defaultBlockSize;
};