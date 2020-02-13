#include "share.h"

#include <iostream>
#include <sstream>

#include <boost/filesystem.hpp>



#include "Config.h"
#include "bayan.h"

// TODO remove Debugging code
//#include "Directory.h"

namespace fs = boost::filesystem;

using std::cout;
using std::endl;

void help() {
	// TODO!!! Write help
	cout << "Welcome bayan! Params: " << endl;
}

uint8_t normlizeBlockSize(const string& sBlockSize) {
	std::istringstream iss(sBlockSize);
	// TODO!!! Поменять blockSize (везде) на size_t
	uint8_t size;
	iss >> size;
	if (size <= 0) {
		help();
		exit(-1);
	}
	return size;
}

Config::SupportedHashTypes normlizeHashType(const string& sHashType) {
	if (sHashType == "crc32") {
		return Config::SupportedHashTypes::CRC32;
	}
	else if (sHashType == "md5") {
		return Config::SupportedHashTypes::MD5;
	}
	else {
		help();
		exit(-1);
	}
}

int main(int argc, char** argv) {

	// TODO make parser arguments with boost
	if (argc < 4) {
		help();
		return -1;
	}

	string sBlockSize = argv[1];
	uint8_t blockSize = normlizeBlockSize(sBlockSize);

	string sHashType = argv[2];
	Config::SupportedHashTypes hashType = normlizeHashType(sHashType);

	vector<string> files;
	for (int i = 3; i < argc; ++i) {
		files.push_back(string{ argv[i] });
	}
	
	// TODO Использовать Config через Bayan
	Config::blockSize = blockSize;
	Config::curHashType = hashType;

	//Config config = Config::getInstance();
	///*config.blockSize = argv[3];*/
	//config.blockSize = 5;
	Config::blockSize = 5;

	// Allow set directories instead files
	//Bayan bayan(3, { "../tests/trivial-test/file1.txt", "../tests/trivial-test/file2.txt" , "../tests/trivial-test/file3.txt" });
	// TODO! blockSize - don't using ??

	// TODO!!!!! Signal on error, when files path
	Bayan bayan(Config::blockSize, files);

	/*bayan.run();*/
	// TODO Delete newlines before output
	bayan.printGroups(cout);

	/*Bayan bayan2(3, "../tests");
	bayan2.printGroups(cout);*/

	return 0;
}
