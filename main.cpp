#include "share.h"

#include <iostream>
#include <sstream>
#include <memory>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "Arguments.h"
#include "BayanConfig.h"
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

SupportedHashTypes normalizeHashType(const string& sHashType) {
	SupportedHashTypes curHashType;
	bool res = BayanConfig::toSupportedHashType(sHashType, curHashType);
	if (!res) {
		help();
		exit(-1);
	}
	return curHashType;
}

int main(int argc, char** argv) {
	Arguments::parse(argc, argv);

	checkBlockSize(Arguments::blockSize);
	SupportedHashTypes hashType = normalizeHashType(Arguments::hashType);
	BayanConfig config{Arguments::blockSize , hashType};

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
