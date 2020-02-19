#include "share.h"

#include <iostream>
#include <sstream>
#include <memory>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "Arguments.h"
#include "Config.h"
#include "Bayan.h"

namespace fs = boost::filesystem;

using std::cout;
using std::endl;

void help() {
	Arguments::showDesc();
}

void checkBlockSize(const size_t& blockSize) {
	if (blockSize <= 0) {
		help();
		error("Bad block size");
	}
}

SupportedHashType normalizeHashType(const string& sHashType) {
	SupportedHashType curHashType;
	bool res = Config::toSupportedHashType(sHashType, curHashType);
	if (!res) {
		help();
		exit(-1);
	}
	return curHashType;
}

int main(int argc, char** argv) {
	Arguments::parse(argc, argv);

	checkBlockSize(Arguments::blockSize);
	SupportedHashType hashType = normalizeHashType(Arguments::hashType);
	Config config{Arguments::blockSize , hashType};

	std::unique_ptr<Bayan> pbayan;
	if (Arguments::dir != "") {
		pbayan = std::make_unique<Bayan>(config, Arguments::dir);
	}
	else {
		pbayan = std::make_unique<Bayan>(config, Arguments::files);
	}

	// TODO Delete newlines before output
	pbayan->printGroups(cout);

	return 0;
}
