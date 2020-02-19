#include "share.h"
#include "Arguments.h"
#include "Config.h"
#include "Bayan.h"

//for debugging
#include <iostream>
using std::cout;
class App {
	void help() {
		Arguments::showDesc();
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

public:
	Config config;

	App(int argc, char** argv) {
		Arguments::parse(argc, argv);
		SupportedHashType hashType = normalizeHashType(Arguments::hashType);
		Config config{ Arguments::blockSize , hashType };
	}

	void run() {
		cout << "-------------\n";
		std::unique_ptr<Bayan> pbayan;
		if (Arguments::dir != "") {
			pbayan = std::make_unique<Bayan>(config, Arguments::dir);
		}
		else {
			pbayan = std::make_unique<Bayan>(config, Arguments::files);
		}
		pbayan->printGroups(cout);
	}
};

int main(int argc, char** argv) {
	App app(argc, argv);
	app.run();
	return 0;
}
