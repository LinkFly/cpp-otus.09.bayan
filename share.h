#pragma once

#include "build/configure.h"

#include <string>
#include <iostream>
#include <cstdint>

using std::cerr;
using std::endl;
using std::string;

#ifdef _MSC_VER
	#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

void error(const string& errMes, int errCode = -1) {
	cerr << errMes << endl;
	exit(errCode);
}

class FileReaded;
class EqualGroup;

using Id = uint8_t;
using PFileReaded = std::shared_ptr<FileReaded>;
using PEqualGroup = std::shared_ptr<EqualGroup>;

enum class SupportedHashType {
	Debug, CRC32, MD5, SHA1
};
