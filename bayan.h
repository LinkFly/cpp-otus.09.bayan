#pragma once

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

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

#include <boost/filesystem.hpp>

#include "FileReaded.h"
#include "Hash.h"
#include "Directory.h"

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
using std::make_unique;
using std::make_shared;

namespace fs = boost::filesystem;

class EqualGroup : public EqualGroupBase {
public:
	friend class EqualGroupBase;
	void add(PFileReaded file) {
		string filePath = file->getFilePath();
		filesSet[filePath] = file;
	}
	~EqualGroup() {
		/*cout << "===================== ~EqualGroup =======================\n";*/
	}
};

class EqualGroupsCollection {
	list<PEqualGroup> equalGroups;
public:

	void add(PEqualGroup& group) {
		equalGroups.push_back(group);
	}
	void forEach(std::function<void(PEqualGroup & group)> fnGroupProcess) {
		for (auto group : equalGroups) {
			fnGroupProcess(group);
		}
	}
};

using Id = uint8_t;
using PFileReaded = shared_ptr<FileReaded>;

class Bayan {
	vector<string> files;
	unsigned blockSize;
	vector<PFileReaded> readedFiles;
	EqualGroupsCollection groupsCol;

	enum class CompareResult : int8_t { Unknown = -1, NotEqual, Equal };

private:
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
				groupsCol.add(eqGroup);
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
			fileLeft->eqGroup->forEachWhile([this, &fileRight, &result](PFileReaded& file) {
				checkTable.save(file->id, fileRight->id, true);
				return true;
				});
			fileRight->eqGroup->forEachWhile([this, &fileLeft, &result](PFileReaded& file) {
				checkTable.save(file->id, fileLeft->id, true);
				return true;
				});
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
		fileLeft->finishReading();
		fileRight->finishReading();
		return res;
	}

	string prepareDir(const string& dir) {
		// TODO checkint on abs
		const fs::path workdir = fs::current_path();
		return (workdir / dir).lexically_normal().string();
	}

public:
	struct Utils {
		static string normalizeFilePath(const string& sPath) {
			return Directory::normalizeFilePath(sPath);
		}
	};

	Bayan(uint8_t blockSize, vector<string> files, bool isNotRun = false): blockSize(blockSize), files(files) {
		/*cout << "Files: \n------------\n" << endl;*/
		for (auto file : files) {
			/*cout << file << endl;*/
			auto fileReaded = std::make_shared<FileReaded>(file);
			readedFiles.push_back(fileReaded);
		}
		cout << endl;
		if (!isNotRun) {
			run();
		}
	}

	Bayan(uint8_t blockSize, string dir, bool isNotRun = false): 
		Bayan(blockSize, Directory::getFiles(prepareDir(dir)), isNotRun) {}

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

	void printGroups(std::ostream& out) {
		groupsCol.forEach([&out](PEqualGroup& group) {
			group->forEachWhile([&out](PFileReaded& file) {
				out << file->getFilePath() << endl;
				return true;
			});
			out << endl;
		});
	}
};
