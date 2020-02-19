#include "share.h"

#define BOOST_TEST_MODULE bayan_test_module

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <functional>
#include <ctime>
#include <sstream>
#include <map>

#include "Bayan.h"

using std::string;
using std::cout;
using std::endl;
using std::clock;

// Utils
bool call_test(string name, std::function<bool(void)> fntest) {
	cout << "------------------------------\n";
	cout << endl << name << ":\n";

	auto startTime = clock();
	bool res = fntest();
	auto endTime = clock();

	cout << "TIME: " << endTime - startTime << "ms" << endl;
	cout << "------------------------------\n";
	return res;
}
//// end Utils

void _printDiffResAndWait(const string& testName, const string& res, const string& waitres) {
	cout
		<< "Failed " << testName
		<< ". \nFact Result:\n-------------------\n"
		<< res << "\n--------------\nWait Result:\n-----------------\n"
		<< waitres << "\n-------------------------\n" << endl;
}


const string relpathPrefix{ "../tests/" };

vector<vector<string>> _getWaitGroupsFiles(const string& testDir, const vector<vector<string>>& waitGroups) {
	vector<vector<string>> res;
	for (vector<string> group : waitGroups) {
		/*size_t idx = 0;*/
		vector<string> curGroup;
		for (string& file : group) {
			auto relFile = relpathPrefix + testDir + "/" + file;
			string curFile = Bayan::Utils::normalizeFilePath(relFile);
			/*group[idx++] = curFile;*/
			curGroup.push_back(curFile);
		}
		res.push_back(curGroup);
	}
	return res;
}

void _getWaitStringFromGroupsFiles(const string& testDir, const vector<vector<string>>& waitGroups, std::ostringstream& sout) {
	auto prepareWaitGroups = _getWaitGroupsFiles(testDir, waitGroups);

	for (vector<string>& group : prepareWaitGroups) {
		// using map because using map into EqualGroup
		std::map<string, bool> waitFiles;
		// TODO to sort groups
		for (auto& file : group) {
			waitFiles[file] = true;
		}
		// Preparing soutWaitRes
		for (auto& sCurFile : waitFiles) {
			sout << std::get<0>(sCurFile) << endl;
		}
		sout << endl;
	}

}

bool _shareTestCode(const string& testDir, const vector<vector<string>>& waitGroups,
	SupportedHashType usingHashType = SupportedHashType::Debug
) {
	Config config;
	config.blockSize = 5;
	config.curHashType = usingHashType;

	std::ostringstream soutRes, soutWaitRes;
	_getWaitStringFromGroupsFiles(testDir, waitGroups, soutWaitRes);

	Bayan bayan(config, relpathPrefix + testDir, true);
	bayan.run();
	bayan.printGroups(soutRes);
	cout << "Result:\n-----------\n" << soutRes.str() << "-------------------" << endl;
	auto res = soutRes.str();
	auto waitRes = soutWaitRes.str();
	auto finalRes = res == waitRes;
	if (!finalRes) {
		_printDiffResAndWait(testDir, res, waitRes);
	}

	return finalRes;
}
bool trivial_test() {
	return call_test(__PRETTY_FUNCTION__, []() {
		return _shareTestCode("trivial_test", vector<vector<string>>
			{
				{"file1.txt", "file3.txt"}
			}
		);
	});
}

bool trivial_test_2_not_eq() {
	return call_test(__PRETTY_FUNCTION__, []() {
		return _shareTestCode("trivial_test_2_not_eq", vector<vector<string>>{});
		});
}

bool trivial_test_2_eq() {
	return call_test(__PRETTY_FUNCTION__, []() {
		return _shareTestCode("trivial_test_2_eq", vector<vector<string>>
		{
			{"file1.txt", "file2.txt"}
		}
		);
		});
}

bool trivial_test_2_groups() {
	return call_test(__PRETTY_FUNCTION__, []() {
		return _shareTestCode("trivial_test_2_groups", vector<vector<string>>
		{
			{"file1.txt", "file2.txt"},
			{ "file3.txt", "file4.txt" }
		}
		);
		});
}

const vector<vector<string>> waitGroupsForRecurseLikeTests = vector<vector<string>>
{
	{"dir1/file1.txt", "dir1/file3.txt", "dir2/subdir21/file1.txt", "dir2/subdir21/file3.txt" },
	{ "dir1/file2.txt", "dir2/subdir21/file2.txt" }
};

bool recurse_test() {
	return call_test(__PRETTY_FUNCTION__, []() {
		return _shareTestCode("recurse_test", waitGroupsForRecurseLikeTests);
	});
}

bool recurse_test_with_crc32() {
	return call_test(__PRETTY_FUNCTION__, []() {
		return _shareTestCode("recurse_test", waitGroupsForRecurseLikeTests, SupportedHashType::CRC32);
	});
}

bool recurse_test_with_md5() {
	return call_test(__PRETTY_FUNCTION__, []() {
		return _shareTestCode("recurse_test", waitGroupsForRecurseLikeTests, SupportedHashType::MD5);
		});
}

bool recurse_test_with_sha1() {
	return call_test(__PRETTY_FUNCTION__, []() {
		return _shareTestCode("recurse_test", waitGroupsForRecurseLikeTests, SupportedHashType::SHA1);
		});
}

void init_base_fixtures() {
	// Init code must be here

}

struct Init {
	Init(std::function<void()> init_func) {
		init_func();
	}
};
#define INIT(init_func) struct Init init(init_func);

BOOST_AUTO_TEST_SUITE(bayan_test_suite)
INIT(init_base_fixtures)

BOOST_AUTO_TEST_CASE(test_of_bayan)
{
	BOOST_CHECK(trivial_test());
	BOOST_CHECK(trivial_test_2_not_eq());
	BOOST_CHECK(trivial_test_2_eq());
	BOOST_CHECK(trivial_test_2_groups());
	BOOST_CHECK(recurse_test());
	BOOST_CHECK(recurse_test_with_crc32());
	BOOST_CHECK(recurse_test_with_md5());
	BOOST_CHECK(recurse_test_with_sha1());
}
BOOST_AUTO_TEST_SUITE_END()
