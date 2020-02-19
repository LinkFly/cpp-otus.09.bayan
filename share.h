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

//enum class loglevel {
//	info, warn, error
//};

//using namespace boost::trivial;
//class logger {
//	string prefix, ending;
//public:
//	enum class loglevel {
//		info, warn, error
//	};
//	logger(const string& prefix = "", const string& ending = "") : prefix{ prefix }, ending{ ending } {
//
//	}
//	
//
//	void operator()(loglevel level, const string& message) {
//		auto curMes = prefix + message + ending;
//		/*BOOST_LOG_TRIVIAL(level) << curMes;*/
//		switch (level) {
//		case loglevel::info: return BOOST_LOG_TRIVIAL(info);
//		}
//		BOOST_LOG_TRIVIAL(info) << "An informational severity message";
//		BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
//		BOOST_LOG_TRIVIAL(error) << "An error severity message";
//	}
//};