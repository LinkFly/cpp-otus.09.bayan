#pragma once

#include <memory>
#include <string>
#include <vector>

using std::string;
using std::vector;



//constexpr vector<string> supportedHashTypes = {
//
//};

class Config {
	static inline Config* instance = nullptr;
	Config() = default;
	
public:
	enum class SupportedHashTypes {
		Debug, CRC32 // ...
		/*static const string Debug = "_debug_";*/
	};
	static inline const bool isDefaultDisableCache = true;
	static inline const uint8_t defaultBlockSize = 5;
	static inline SupportedHashTypes curHashType = SupportedHashTypes::Debug;
	static inline uint8_t blockSize = defaultBlockSize;

	//static Config& getInstance() {
	//	// TODO!!! make safe this code
	//	if (instance == nullptr) {
	//		instance = new Config{};
	//	}
	//	return *instance;
	//}

	//static void destroy() {
	//	delete instance;
	//}
};