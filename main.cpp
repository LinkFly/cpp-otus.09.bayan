#include "share.h"
#include "share-types.h"
#include "Arguments.h"
#include "Config.h"
#include "Bayan.h"

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
		std::unique_ptr<Bayan> pbayan;
		if (Arguments::dir != "") {
			pbayan = std::make_unique<Bayan>(config, Arguments::dir);
		}
		else {
			pbayan = std::make_unique<Bayan>(config, Arguments::files);
		}
		// TODO Delete newlines before output
		pbayan->printGroups(cout);
	}
};

int main(int argc, char** argv) {
	App app(argc, argv);
	app.run();
	return 0;
}
