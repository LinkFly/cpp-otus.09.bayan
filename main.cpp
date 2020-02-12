#include "share.h"

#include <iostream>

//#include <filesystem>
#include <boost/filesystem.hpp>

#include "Config.h"
#include "bayan.h"

//namespace fs = std::filesystem;
namespace fs = boost::filesystem;

using std::cout;
using std::endl;

int main(int argc, char** argv) {
	Config config = Config::getInstance();
	/*config.blockSize = argv[3];*/
	config.blockSize = 5;

	Bayan bayan(3, { "../tests/trivial-test/file1.txt", "../tests/trivial-test/file2.txt" , "../tests/trivial-test/file3.txt" });

	bayan.run();
	bayan.printGroups(cout);

	Config::destroy();
	return 0;
}
