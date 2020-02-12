#pragma once

#include <stdint.h>

class FileReaded;
class EqualGroup;

using Id = uint8_t;
using PFileReaded = std::shared_ptr<FileReaded>;
using PEqualGroup = std::shared_ptr<EqualGroup>;

