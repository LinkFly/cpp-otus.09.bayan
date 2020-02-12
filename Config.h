#pragma once

#include <memory>
#include <string>

using std::string;

const uint8_t defaultBlockSize = 5;

class Config {
	static inline Config* instance = nullptr;
	Config() = default;
	
public:
	string curHashType = "_debug_";
	uint8_t blockSize = defaultBlockSize;

	static Config& getInstance() {
		// TODO!!! make safe this code
		if (instance == nullptr) {
			instance = new Config{};
		}
		return *instance;
	}

	static void destroy() {
		delete instance;
	}
};