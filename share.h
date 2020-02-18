#pragma once

#include <string>
#include <iostream>

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