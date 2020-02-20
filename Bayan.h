#pragma once

#include "share.h"

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <functional>
#include <map>
#include <set>
#include <fstream>
#include <tuple>
#include <memory>

#include <boost/filesystem.hpp>

#include "Config.h"
#include "FileReaded.h"
#include "EqualGroup.h"
#include "Hash.h"
#include "FilesUtils.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::map;
using std::ifstream;
using std::list;

using std::unique_ptr;
using std::shared_ptr;
using std::make_shared;

namespace fs = boost::filesystem;

using Id = uint8_t;
using PFileReaded = shared_ptr<FileReaded>;

class Bayan {
	const Config& config;
	vector<PFileReaded> readedFiles;
	list<PEqualGroup> equalGroups;

	enum class CompareResult : int8_t { Unknown = -1, NotEqual, Equal };
	class CheckTable {
		map<std::tuple<Id, Id>, bool> negTable;
	public:
		void save(Id idLeft, Id idRight, bool isEqual) {
			negTable[std::make_tuple(idLeft, idRight)] = isEqual;
			negTable[std::make_tuple(idRight, idLeft)] = isEqual;
		}
		CompareResult is(Id idLeft, Id idRight) {
			auto ids = std::make_tuple(idLeft, idRight);
			auto kvIter = negTable.find(ids);
			if (kvIter == negTable.end()) {
				return CompareResult::Unknown;
			}
			return kvIter->second ? CompareResult::Equal : CompareResult::NotEqual;
		}
	} checkTable;

	void saveCompareResult(PFileReaded& fileLeft, PFileReaded& fileRight, bool result) {
		checkTable.save(fileLeft->id, fileRight->id, result);
		if (result) {
			if (fileLeft->isEmptyGroup() && fileLeft->isEmptyGroup()) {
				PEqualGroup eqGroup = std::make_shared<EqualGroup>();
				equalGroups.push_back(eqGroup);
				eqGroup->add(fileLeft);
				eqGroup->add(fileRight);
				fileLeft->eqGroup = eqGroup;
				fileRight->eqGroup = eqGroup;
			}
			else {
				if (!fileLeft->isEmptyGroup()) {
					fileLeft->eqGroup->add(fileRight);
					fileRight->eqGroup = fileLeft->eqGroup;
				}
				if (!fileRight->isEmptyGroup()) {
					fileRight->eqGroup->add(fileLeft);
					fileLeft->eqGroup = fileRight->eqGroup;
				}
			}
			auto fnForWhile = [this, &fileRight](PFileReaded& file) -> bool {
				checkTable.save(file->id, fileRight->id, true);
				return true;
			};
			fileLeft->eqGroup->forEachWhile(fnForWhile);
			auto forForWhile2 = [this, &fileLeft](PFileReaded& file) {
				checkTable.save(file->id, fileLeft->id, true);
				return true;
			};
			fileRight->eqGroup->forEachWhile(forForWhile2);
		}
	}

	CompareResult checkCompareResult(
		PFileReaded& fileLeft, PFileReaded& fileRight, list<PFileReaded> history = list<PFileReaded>{}
	) {
		return checkTable.is(fileLeft->id, fileRight->id);
	}

	bool compareFilesByBlocks(PFileReaded& fileLeft, PFileReaded& fileRight) {
		if (fileLeft->getSize() != fileRight->getSize()) {
			return false;
		}

		shared_ptr<Hash> hashLeft, hashRight;
		fileLeft->startReading();
		fileRight->startReading();
		bool res = true;
		while (fileLeft->readNextHash(hashLeft) && fileRight->readNextHash(hashRight)) {
			if (*hashLeft != *hashRight) {
				res = false;
				break;
			}
		}
		fileLeft->finishReadingIfNotFinished();
		fileRight->finishReadingIfNotFinished();
		return res;
	}

	string prepareDir(const string& dir) {
		auto dirPath = fs::path{ dir };
		if (!dirPath.is_absolute()) {
			dirPath = (fs::current_path() / dirPath);
		}
		return (boost::filesystem::pathExt{ dirPath }).lexically_normal().string();
	}

	void compareFiles(PFileReaded& fileLeft, PFileReaded& fileRight) {
		CompareResult checkRes = checkCompareResult(fileLeft, fileRight);
		bool isEqual;
		if (checkRes != CompareResult::Unknown) {
			isEqual = checkRes == CompareResult::Equal;
		}
		else {
			isEqual = compareFilesByBlocks(fileLeft, fileRight);
			saveCompareResult(fileLeft, fileRight, isEqual);
		}
	}

public:
	struct Utils {
		static string normalizeFilePath(const string& sPath) {
			return FilesUtils::normalizeFilePath(sPath);
		}
	};

	Bayan(const Config& config, vector<string> files, bool isNotRun = false) : config(config) {
		/*FileReaded::init();*/
		for (auto file : files) {
			auto fileReaded = std::make_shared<FileReaded>(file, config);
			readedFiles.push_back(fileReaded);
		}
		if (!isNotRun) {
			run();
		}
	}

	Bayan(const Config& config, string dir, bool isNotRun = false) :
		Bayan(config, FilesUtils::getFiles(prepareDir(dir)), isNotRun) {}

	void run() {
		size_t size = readedFiles.size();
		for (size_t i = 0; i < size - 1; i++) {
			auto curLeftFile = readedFiles[i];
			for (size_t j = i + 1; j < size; j++) {
				compareFiles(curLeftFile, readedFiles[j]);
			}
		}
		// After we need reset groups into files because unblock smart pointers
		for (auto& file : readedFiles) {
			file->eqGroup.reset();
		}
	}
	void printGroups(std::ostream& out) {
		for (auto group : equalGroups) {
			group->forEachWhile([&out](PFileReaded& file) {
				out << file->getFilePath() << endl;
				return true;
				});
			out << endl;
		}
	}
};
