#include "share.h"

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/crc.hpp>

#include "Config.h"
#include "bayan.h"

// TODO remove Debugging code
#include <map>
using std::map;
//#include "Directory.h"

namespace fs = boost::filesystem;

using std::cout;
using std::endl;

int main(int argc, char** argv) {
	//map<int, double> map;
	//auto kv = map.find(3);
	//
	//
	////auto kv = map.at(3);
	//cout << std::get<0>(kv) << endl;



	//Config config = Config::getInstance();
	///*config.blockSize = argv[3];*/
	//config.blockSize = 5;
	Config::blockSize = 5;

	//Bayan bayan(3, { "../tests/trivial-test/file1.txt", "../tests/trivial-test/file2.txt" , "../tests/trivial-test/file3.txt" });

	///*bayan.run();*/
	//bayan.printGroups(cout);

	Bayan bayan2(3, "../tests");

	return 0;
}
