#pragma once

#include "share.h"

#include <iostream>
#include <chrono>
#include <string>

using std::ostream;
using std::endl;

// Uncomment it for forcing logs (usual configure it when to use cmake)
//#define LOGS 1

struct Logger {
	// [[maybe_unused]] - since C++17
	Logger(/*[[maybe_unused]]*/ ostream& out = std::cout, std::string prefix = "", std::string suffix = "", std::string ending = "") :
		_prefix(prefix), _suffix(suffix), _ending(ending), _out(out) {
	}
	template<typename Arg, typename ...Args>
	void operator()(Arg arg, Args&&... args) {
#ifdef LOGS
		std::string cur_prefix;
		if (_prefix != "") {
			cur_prefix = _prefix;
		}
		else {
			auto now = std::chrono::system_clock::now();
			cur_prefix = "[" + std::to_string(now.time_since_epoch().count()) + "] ";
		}
		_out << cur_prefix << arg;
		((_out << _suffix << args), ...);
		_out << _ending << endl;
#endif
	}
private:
	ostream& _out;
	std::string _prefix;
	std::string _suffix;
	std::string _ending;
};

Logger logger{std::cout, "", "  "};
