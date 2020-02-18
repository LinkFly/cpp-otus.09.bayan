#pragma once

#include <stdint.h>
#include <vector>

class FileReaded;
class EqualGroup;

using Id = uint8_t;
using PFileReaded = std::shared_ptr<FileReaded>;
using PEqualGroup = std::shared_ptr<EqualGroup>;

enum class SupportedHashTypes {
	Debug, CRC32, MD5, SHA1
};
