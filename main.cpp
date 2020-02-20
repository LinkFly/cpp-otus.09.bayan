#include "share.h"
#include "Arguments.h"
#include "Config.h"
#include "Bayan.h"

#include <iostream>
using std::cout;
class App {
	Arguments arguments{};
public:
	Config config{};
private:

	void help() {
		arguments.showDesc();
	}

	SupportedHashType normalizeHashType(const string& sHashType) {
		SupportedHashType curHashType;
		bool res = config.toSupportedHashType(sHashType, curHashType);
		if (!res) {
			help();
			exit(-1);
		}
		return curHashType;
	}

public:
	

	App(int argc, char** argv) {
		arguments.parse(argc, argv);
		SupportedHashType hashType = normalizeHashType(arguments.hashType);
		Config config{ arguments.blockSize , hashType };
		
	}

	void run() {
		std::unique_ptr<Bayan> pbayan;
		if (arguments.dir != "") {
		  // pbayan = std::make_unique<Bayan>(config, arguments.dir); // make_unique since C++17
		  pbayan.reset(new Bayan(config, arguments.dir));
		}
		else {
		  // pbayan = std::make_unique<Bayan>(config, arguments.files); // make_unique since C++17
		  pbayan.reset(new Bayan(config, arguments.files));
		}
		pbayan->printGroups(cout);
	}
};

int main(int argc, char** argv) {
	App app(argc, argv);
	app.run();
	return 0;
}
