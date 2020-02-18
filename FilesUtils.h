#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

using std::vector;
using std::string;
using std::cout;
using std::endl;


class FilesUtils {
public:
	static vector<string> getFiles(const string& dir) {
		vector<string> res;
		for (const fs::path& file : fs::recursive_directory_iterator(dir)) {
			if (fs::is_regular_file(file)) {
				string sFile = file.lexically_normal().generic_string();
				res.push_back(sFile);
			}
		}
		return res;
	}

	static string normalizeFilePath(const string& sPath) {
		fs::path fsPath = fs::path{ sPath };
		if (fsPath.is_absolute()) {
			return fsPath.generic_string();
		}
		fs::path workdir = fs::current_path();
		return (workdir / fsPath).lexically_normal().generic_string();
	}
};
